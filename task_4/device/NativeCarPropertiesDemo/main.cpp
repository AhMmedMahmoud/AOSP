#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <aidl/android/hardware/automotive/vehicle/IVehicle.h>
#include <aidl/android/hardware/automotive/vehicle/BnVehicleCallback.h>
#include <aidl/android/hardware/automotive/vehicle/VehiclePropValue.h>
#include <aidl/android/hardware/automotive/vehicle/VehiclePropValues.h>
#include <aidl/android/hardware/automotive/vehicle/SubscribeOptions.h>
#include <aidl/android/hardware/automotive/vehicle/GetValueRequest.h>
#include <aidl/android/hardware/automotive/vehicle/GetValueRequests.h>
#include <aidl/android/hardware/automotive/vehicle/GetValueResult.h>
#include <aidl/android/hardware/automotive/vehicle/GetValueResults.h>
#include <aidl/android/hardware/automotive/vehicle/SetValueRequest.h>
#include <aidl/android/hardware/automotive/vehicle/SetValueRequests.h>
#include <aidl/android/hardware/automotive/vehicle/SetValueResult.h>
#include <aidl/android/hardware/automotive/vehicle/SetValueResults.h>
#include <aidl/android/hardware/automotive/vehicle/VehiclePropError.h>
#include <aidl/android/hardware/automotive/vehicle/VehiclePropErrors.h>
#include <aidl/android/hardware/automotive/vehicle/StatusCode.h>
#include <mutex>
#include <iostream>
#include <thread>
#include <unistd.h>

using aidl::android::hardware::automotive::vehicle::IVehicle;
using aidl::android::hardware::automotive::vehicle::BnVehicleCallback;
using aidl::android::hardware::automotive::vehicle::VehiclePropValue;
using aidl::android::hardware::automotive::vehicle::VehiclePropValues;
using aidl::android::hardware::automotive::vehicle::SubscribeOptions;
using aidl::android::hardware::automotive::vehicle::StatusCode;
using aidl::android::hardware::automotive::vehicle::GetValueRequest;
using aidl::android::hardware::automotive::vehicle::GetValueRequests;
using aidl::android::hardware::automotive::vehicle::GetValueResult;
using aidl::android::hardware::automotive::vehicle::GetValueResults;
using aidl::android::hardware::automotive::vehicle::SetValueRequest;
using aidl::android::hardware::automotive::vehicle::SetValueRequests;
using aidl::android::hardware::automotive::vehicle::SetValueResult;
using aidl::android::hardware::automotive::vehicle::SetValueResults;
using aidl::android::hardware::automotive::vehicle::VehiclePropError;
using aidl::android::hardware::automotive::vehicle::VehiclePropErrors;

using ndk::ScopedAStatus;

// Custom properties 
constexpr int32_t STATIC_PROP = 0x21402b00;
constexpr int32_t DYNAMIC_PROP = 0x21402b01;

// Callback implementation for property change notifications
class VehicleCallbackImpl : public BnVehicleCallback {
private:
    std::mutex mMutex;
    int32_t mStaticValue;
    int32_t mDynamicValue;
    bool mStaticReceived;
    bool mDynamicReceived;

public:
    VehicleCallbackImpl() 
        : mStaticValue(0), 
          mDynamicValue(0),
          mStaticReceived(false),
          mDynamicReceived(false) {
    }

    ScopedAStatus onGetValues(const GetValueResults& results) override {
        std::lock_guard<std::mutex> lock(mMutex);
        
        for (const auto& result : results.payloads) {
            if (result.status == StatusCode::OK) {
                if (result.prop.has_value()) {
                    const auto& propValue = result.prop.value();
                    
                    if (propValue.prop == STATIC_PROP) {
                        if (!propValue.value.int32Values.empty()) {
                            mStaticValue = propValue.value.int32Values[0];
                            mStaticReceived = true;
                            LOG(INFO) << "Initial Static value: " << mStaticValue;
                        }
                    } else if (propValue.prop == DYNAMIC_PROP) {
                        if (!propValue.value.int32Values.empty()) {
                            mDynamicValue = propValue.value.int32Values[0];
                            mDynamicReceived = true;
                            LOG(INFO) << "Initial Dynamic value: " << mDynamicValue;
                        }
                    }
                }
            } else {
                LOG(ERROR) << "Get property failed with status: " 
                          << static_cast<int>(result.status)
                          << " for prop: " << result.prop.value().prop;
            }
        }
        return ScopedAStatus::ok();
    }

    ScopedAStatus onSetValues(const SetValueResults& results) override {
        for (const auto& result : results.payloads) {
            if (result.status == StatusCode::OK) {
                LOG(INFO) << "Property set successfully, requestId: " << result.requestId;
            } else {
                LOG(ERROR) << "Set property failed with status: " 
                          << static_cast<int>(result.status);
            }
        }
        return ScopedAStatus::ok();
    }

    ScopedAStatus onPropertyEvent(const VehiclePropValues& propValues,
                                   int32_t sharedMemoryFileCount) override {
        std::lock_guard<std::mutex> lock(mMutex);
        
        for (const auto& propValue : propValues.payloads) {
            if (propValue.prop == STATIC_PROP) {
                if (!propValue.value.int32Values.empty()) {
                    mStaticValue = propValue.value.int32Values[0];
                    mStaticReceived = true;
                    LOG(INFO) << "Static property changed: " << mStaticValue;
                    displayStatus();
                }
            } else if (propValue.prop == DYNAMIC_PROP) {
                if (!propValue.value.int32Values.empty()) {
                    mDynamicValue = propValue.value.int32Values[0];
                    mDynamicReceived = true;
                    LOG(INFO) << "Dynamic property changed: " << mDynamicValue;
                    displayStatus();
                }
            }
        }
        
        return ScopedAStatus::ok();
    }

    ScopedAStatus onPropertySetError(const VehiclePropErrors& errors) override {
        LOG(ERROR) << "onPropertySetError called with " << errors.payloads.size() << " errors";
        
        for (const auto& error : errors.payloads) {
            LOG(ERROR) << "Property set error - PropId: 0x" << std::hex << error.propId
                      << ", AreaId: " << error.areaId
                      << ", Status: " << static_cast<int>(error.errorCode);
        }
        return ScopedAStatus::ok();
    }

    void displayStatus() {
        std::cout << "\n========================================" << std::endl;
        std::cout << "        CAR PROPERTIES STATUS           " << std::endl;
        std::cout << "========================================" << std::endl;
        
        if (mStaticReceived) {
            std::cout << "Static Property:  " << mStaticValue << std::endl;
        } else {
            std::cout << "Static Property:  (waiting...)" << std::endl;
        }
        
        if (mDynamicReceived) {
            std::cout << "Dynamic Property: " << mDynamicValue << std::endl;
        } else {
            std::cout << "Dynamic Property: (waiting...)" << std::endl;
        }
        
        std::cout << "========================================\n" << std::endl;
    }

    void getStatus(int32_t& staticVal, int32_t& dynamicVal, bool& staticRcvd, bool& dynamicRcvd) {
        std::lock_guard<std::mutex> lock(mMutex);
        staticVal = mStaticValue;
        dynamicVal = mDynamicValue;
        staticRcvd = mStaticReceived;
        dynamicRcvd = mDynamicReceived;
    }
};

class VhalClient {
private:
    std::shared_ptr<IVehicle> mVehicle;
    std::shared_ptr<VehicleCallbackImpl> mCallback;

public:
    bool init() {
        LOG(INFO) << "========================================";
        LOG(INFO) << "  Initializing VHAL Client";
        LOG(INFO) << "========================================";
        
        // Get VHAL service
        const std::string instance = std::string() + IVehicle::descriptor + "/default";
        LOG(INFO) << "Looking for VHAL service: " << instance;
        
        mVehicle = IVehicle::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(instance.c_str())));
        
        if (!mVehicle) {
            LOG(ERROR) << "Failed to get VHAL service!";
            LOG(ERROR) << "Make sure the vehicle HAL service is running.";
            return false;
        }
        
        LOG(INFO) << "VHAL service connected successfully";
        
        // Create callback
        mCallback = ndk::SharedRefBase::make<VehicleCallbackImpl>();
        LOG(INFO) << "Callback created";
        
        return true;
    }

    std::shared_ptr<VehicleCallbackImpl> getCallback() {
        return mCallback;
    }

    // Get a property value
    bool getProperty(int32_t propId, int32_t areaId = 0) {
        if (!mVehicle || !mCallback) {
            LOG(ERROR) << "VHAL service not initialized";
            return false;
        }

        GetValueRequest request;
        request.prop.prop = propId;
        request.prop.areaId = areaId;
        request.requestId = propId; // Use propId as requestId for tracking

        GetValueRequests requests;
        requests.payloads.push_back(request);

        LOG(INFO) << "Getting property 0x" << std::hex << propId << std::dec;
        auto status = mVehicle->getValues(mCallback, requests);
        
        if (!status.isOk()) {
            LOG(ERROR) << "Failed to get property 0x" << std::hex << propId 
                      << ": " << status.getMessage();
            return false;
        }

        return true;
    }

    // Subscribe to property changes
    bool subscribeProperty(int32_t propId, float sampleRate = 0.0f, int32_t areaId = 0) {
        if (!mVehicle || !mCallback) {
            LOG(ERROR) << "VHAL service not initialized";
            return false;
        }

        SubscribeOptions options;
        options.propId = propId;
        if (areaId != 0) {
            options.areaIds = {areaId};
        }
        options.sampleRate = sampleRate;

        std::vector<SubscribeOptions> optionsList = {options};

        LOG(INFO) << "Subscribing to property 0x" << std::hex << propId << std::dec;
        if (sampleRate > 0) {
            LOG(INFO) << "  Sample rate: " << sampleRate << " Hz";
        } else {
            LOG(INFO) << "  Mode: ON_CHANGE";
        }
        
        auto status = mVehicle->subscribe(mCallback, optionsList, -1);
        
        if (!status.isOk()) {
            LOG(ERROR) << "Failed to subscribe to property 0x" << std::hex << propId 
                      << ": " << status.getMessage();
            return false;
        }

        LOG(INFO) << "Successfully subscribed to property 0x" << std::hex << propId;
        return true;
    }

    // Unsubscribe from property changes
    bool unsubscribeProperty(int32_t propId) {
        if (!mVehicle || !mCallback) {
            LOG(ERROR) << "VHAL service not initialized";
            return false;
        }

        LOG(INFO) << "Unsubscribing from property 0x" << std::hex << propId;
        auto status = mVehicle->unsubscribe(mCallback, {propId});
        
        if (!status.isOk()) {
            LOG(ERROR) << "Failed to unsubscribe from property 0x" << std::hex << propId 
                      << ": " << status.getMessage();
            return false;
        }

        LOG(INFO) << "Successfully unsubscribed from property 0x" << std::hex << propId;
        return true;
    }
};

// Main application
int main() {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════╗\n";
    std::cout << "║   Car Properties Demo (Native C++)    ║\n";
    std::cout << "╚════════════════════════════════════════╝\n";
    std::cout << "\n";

    // Initialize binder thread pool
    ABinderProcess_setThreadPoolMaxThreadCount(4);
    ABinderProcess_startThreadPool();

    LOG(INFO) << "Starting VHAL Client Demo...";

    VhalClient client;
    
    if (!client.init()) {
        LOG(ERROR) << "Failed to initialize VHAL client";
        LOG(ERROR) << "Exiting...";
        return -1;
    }

    LOG(INFO) << "\n========================================";
    LOG(INFO) << "  Setting up property monitoring";
    LOG(INFO) << "========================================";

    // Get initial values
    LOG(INFO) << "\nReading initial property values...";
    client.getProperty(STATIC_PROP);
    sleep(1);
    client.getProperty(DYNAMIC_PROP);
    sleep(1);

    // Subscribe to static property (ON_CHANGE)
    LOG(INFO) << "\nSubscribing to STATIC property (ON_CHANGE)...";
    if (!client.subscribeProperty(STATIC_PROP, 0.0f)) {
        LOG(WARNING) << "Failed to subscribe to static property";
    }
    sleep(1);

    // Subscribe to dynamic property (NORMAL rate - continuous updates)
    LOG(INFO) << "\nSubscribing to DYNAMIC property (NORMAL rate)...";
    if (!client.subscribeProperty(DYNAMIC_PROP, 1.0f)) {
        LOG(WARNING) << "Failed to subscribe to dynamic property";
    }
    sleep(1);

    LOG(INFO) << "\n========================================";
    LOG(INFO) << "  Monitoring started successfully!";
    LOG(INFO) << "========================================";
    LOG(INFO) << "\nProperty IDs being monitored:";
    LOG(INFO) << "  - Static Property:  0x" << std::hex << STATIC_PROP;
    LOG(INFO) << "  - Dynamic Property: 0x" << std::hex << DYNAMIC_PROP;
    LOG(INFO) << std::dec;
    
    std::cout << "\nWaiting for property updates..." << std::endl;
    std::cout << "(Press Ctrl+C to exit)\n" << std::endl;

    // Display initial status
    sleep(2);
    auto callback = client.getCallback();
    if (callback) {
        callback->displayStatus();
    }

    // Keep the process running to receive callbacks
    LOG(INFO) << "Running continuously... Press Ctrl+C to exit";
    
    // Optional: Periodic status display
    std::thread statusThread([&callback]() {
        while (true) {
            sleep(10);  // Display status every 10 seconds
            if (callback) {
                int32_t staticVal, dynamicVal;
                bool staticRcvd, dynamicRcvd;
                callback->getStatus(staticVal, dynamicVal, staticRcvd, dynamicRcvd);
                
                if (staticRcvd || dynamicRcvd) {
                    LOG(INFO) << "Periodic status check - Static: " << staticVal 
                             << ", Dynamic: " << dynamicVal;
                }
            }
        }
    });
    statusThread.detach();

    ABinderProcess_joinThreadPool();

    return 0;
}