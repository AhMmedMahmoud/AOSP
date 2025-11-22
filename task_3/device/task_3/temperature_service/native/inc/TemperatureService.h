#ifndef TEMPERATURE_SERVICE_H
#define TEMPERATURE_SERVICE_H

#include <aidl/android/app/sensortask/IReadingsCallback.h>
#include <aidl/android/app/sensortask/TemperatureReading.h>
#include <aidl/android/app/sensortask/BnTemperatureService.h>
#include "SensorManager.h"
#include "CallbackManager.h"

namespace tsapinamespace = ::aidl::android::app::sensortask;

class TemperatureService : public tsapinamespace::BnTemperatureService
{
public:
    TemperatureService();
    ~TemperatureService();

    bool init();

    // Override AIDL methods
    ndk::ScopedAStatus getAllTemperatureReadings(
        std::vector<tsapinamespace::TemperatureReading> *_aidl_return) override;

    ndk::ScopedAStatus setCurrentVoltage(double voltage_value) override;

    ndk::ScopedAStatus getAllReadingAsync(
        const std::shared_ptr<tsapinamespace::IReadingsCallback> &callback) override;

    ndk::ScopedAStatus registerCallback(
        const std::shared_ptr<tsapinamespace::IReadingsCallback> &callback) override;

    ndk::ScopedAStatus unregisterCallback(
        const std::shared_ptr<tsapinamespace::IReadingsCallback> &callback) override;

    const std::string getServiceName(void)
    {
        return std::string() + descriptor + "/default";
    }

private:
    SensorManager mSensor;
    CallbackManager mCallbacks;
};

#endif // TEMPERATURE_SERVICE_H
