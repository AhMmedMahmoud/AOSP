#include <aidl/android/app/sensortask/ITemperatureService.h>
#include <aidl/android/app/sensortask/TemperatureReading.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <utils/Log.h>
#include <iostream>
#include <vector>


using namespace aidl::android::app::sensortask;

int main() {
    ALOGW("Temperature Service user app started.\n");

    const char* serviceName = "android.app.sensortask.ITemperatureService/default";

    // Get binder
    auto binder = ndk::SpAIBinder(AServiceManager_getService(serviceName));
    if (binder.get() == nullptr) {
		ALOGE("Failed to get service: %s", serviceName);
		return -1;
	}

    std::shared_ptr<ITemperatureService> service = ITemperatureService::fromBinder(binder);
    if (service != nullptr)
	{	
		ALOGW("%s:%d: Connect ITemperatureService API successful", __FUNCTION__, __LINE__);

		// Call setCurrentVoltage()
		double voltage = 2.5;
		auto status = service->setCurrentVoltage(voltage);
		if (!status.isOk()) {
			ALOGE("setCurrentVoltage failed: %s", status.getDescription().c_str());
		} else {
			ALOGW("setCurrentVoltage(%f) called successfully", voltage);
		}

		// Get all readings
		std::vector<TemperatureReading> readings;
		status = service->getAllTemperatureReadings(&readings);
		if (!status.isOk()) {
			ALOGE("getAllTemperatureReadings failed: %s", status.getDescription().c_str());
			return -1;
		}

		ALOGW("Received %zu readings", readings.size());
		for (size_t i = 0; i < readings.size(); ++i) {
			ALOGW("Reading[%zu]: value=%.2f, timestamp=%ld",
				i, readings[i].value, readings[i].timestamp);
		}
	}
	else 
	{
        ALOGE("Failed to cast binder to ITemperatureService");
        return -1;
    }
    
    return 0;
}
