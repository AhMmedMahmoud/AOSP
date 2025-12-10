void FakeVehicleHardware::init(int32_t s2rS2dConfig) {
    maybeGetGrpcServiceInfo(&mPowerControllerServiceAddress);

    {
        ....
    }

    // Register custom vendor properties
    {
        VehicleAreaConfig areaConfig{};
        areaConfig.areaId = 0;

        // 0x21402b00 - static read-only
        VehiclePropConfig staticProp{};
        staticProp.prop = toInt(TestVendorProperty::STATIC_READ_ONLY_PROPERTY);
        staticProp.access = VehiclePropertyAccess::READ;
        staticProp.areaConfigs.push_back(areaConfig);
        staticProp.changeMode = VehiclePropertyChangeMode::ON_CHANGE;
        mServerSidePropStore->registerProperty(staticProp, nullptr);
        mVendorPropsValues[staticProp.prop] = 12;

        // 0x21402b01 - dynamic read/write
        VehiclePropConfig dynamicProp{};
        dynamicProp.prop = toInt(TestVendorProperty::DYNAMIC_READ_WRITE_PROPERTY);
        dynamicProp.access = VehiclePropertyAccess::READ_WRITE;
        dynamicProp.areaConfigs.push_back(areaConfig);
        dynamicProp.changeMode = VehiclePropertyChangeMode::ON_CHANGE;
        mServerSidePropStore->registerProperty(dynamicProp, nullptr);
        mVendorPropsValues[dynamicProp.prop] = 13;
    }

    ...
}


FakeVehicleHardware::ValueResultType FakeVehicleHardware::maybeGetSpecialValue(
        const VehiclePropValue& value, bool* isSpecialValue) const {
    *isSpecialValue = false;
    int32_t propId = value.prop;
    ValueResultType result;

    if (mPowerControllerServiceAddress != "") {
        if (mPowerPropIds.find(propId) != mPowerPropIds.end()) {
            *isSpecialValue = true;
            return getPowerPropFromExternalService(propId);
        }
    }

    if(propId == toInt(TestVendorProperty::STATIC_READ_ONLY_PROPERTY) || 
       propId == toInt(TestVendorProperty::DYNAMIC_READ_WRITE_PROPERTY)) 
    {
        ALOGD("FakeVehicleHardware::maybeGetSpecialValue %d", propId);
        *isSpecialValue = true;
        int32_t storedValue; 
        {
            std::scoped_lock lock(mVendorPropsLock);
            storedValue = mVendorPropsValues[propId];
        }
        result = mValuePool->obtainInt32(storedValue);

        result.value()->prop = propId;
        result.value()->areaId = 0;
        result.value()->timestamp = elapsedRealtimeNano();
        return result;
    }
    ...
}

VhalResult<void> FakeVehicleHardware::maybeSetSpecialValue(const VehiclePropValue& value,
                                                           bool* isSpecialValue) {
    *isSpecialValue = false;
    VehiclePropValuePool::RecyclableType updatedValue;
    int32_t propId = value.prop;

    if(propId == toInt(TestVendorProperty::STATIC_READ_ONLY_PROPERTY) || 
       propId == toInt(TestVendorProperty::DYNAMIC_READ_WRITE_PROPERTY)) 
    {
        ALOGD("FakeVehicleHardware::maybeSetSpecialValue %d", propId);
        *isSpecialValue = true;

        int32_t newValue = 0;
        if (!value.value.int32Values.empty()) {
            newValue = value.value.int32Values[0];
        }

        {
            std::scoped_lock lock(mVendorPropsLock);
            mVendorPropsValues[propId] = newValue;
        }

        // Create property change event
        auto updatedValue = mValuePool->obtainInt32(newValue);
        updatedValue->prop = propId;
        updatedValue->areaId = value.areaId;
        updatedValue->timestamp = elapsedRealtimeNano();
        onValueChangeCallback(*updatedValue);
        return {};
    }
    ...
}