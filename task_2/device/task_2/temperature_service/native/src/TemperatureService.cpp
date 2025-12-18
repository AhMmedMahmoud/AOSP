#include "TemperatureService.h"
#include <thread>
#include <chrono>
#include <iostream>

using tsapinamespace::IReadingsCallback;
using tsapinamespace::TemperatureReading;

TemperatureService::TemperatureService() {}
TemperatureService::~TemperatureService() {}

bool TemperatureService::init() {
    return mSensor.initialize();
}

ndk::ScopedAStatus TemperatureService::getAllTemperatureReadings(std::vector<TemperatureReading>* _aidl_return) {
    auto data = mSensor.getReadings();
    _aidl_return->clear();

    for (const auto& d : data) {
        TemperatureReading t;
        t.value = d.temperature;
        t.timestamp = d.timestamp;
        _aidl_return->push_back(t);
    }

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus TemperatureService::getAllReadingAsync(const std::shared_ptr<IReadingsCallback>& callback) {
    std::thread([this, callback]() {
        std::vector<TemperatureReading> out;
        auto data = mSensor.getReadings();
        for (const auto& d : data) {
            TemperatureReading t;
            t.value = d.temperature;
            t.timestamp = d.timestamp;
            out.push_back(t);
        }
        callback->onResult(out);
    }).detach();

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus TemperatureService::setCurrentVoltage(double voltage_value) {
    mSensor.setVoltage(voltage_value);

    double temperature = 9.48 * voltage_value + 1.5;
    
    std::vector<TemperatureReading> readings;
    auto data = mSensor.getReadings();
    for (const auto& d : data) {
        TemperatureReading t;
        t.value = d.temperature;
        t.timestamp = d.timestamp;
        readings.push_back(t);
    }

    std::thread([this, readings = std::move(readings)]() mutable {
        mCallbacks.notifyAll(readings);
    }).detach();

    return ndk::ScopedAStatus::ok();  
}

ndk::ScopedAStatus TemperatureService::registerCallback(
    const std::shared_ptr<tsapinamespace::IReadingsCallback>& callback) 
{
    mCallbacks.registerCallback(callback);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus TemperatureService::unregisterCallback(
    const std::shared_ptr<tsapinamespace::IReadingsCallback>& callback) 
{
    mCallbacks.unregisterCallback(callback);
    return ndk::ScopedAStatus::ok();
}