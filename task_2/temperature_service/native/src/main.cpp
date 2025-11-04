#include "TemperatureService.h"
#include <utils/Log.h>
#include <iostream>
#include <signal.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "temperature-service"

namespace tsnamespace = aidl::temperature::service::api;

int main()
{
    ALOGI("%s:%d: Starting TemperatureService", __FUNCTION__, __LINE__);

    signal(SIGPIPE, SIG_IGN);

    ABinderProcess_setThreadPoolMaxThreadCount(4);

    std::shared_ptr<TemperatureService> serviceObj = 
        ndk::SharedRefBase::make<TemperatureService>();

    if (!serviceObj)
    {
        ALOGE("%s:%d: Failed to create TemperatureService instance", __FUNCTION__, __LINE__);
        return -1;
    }

    // Initialize sensor manager
    if (!serviceObj->init())
    {
        ALOGW("%s:%d: SensorManager init failed, service will still register", __FUNCTION__, __LINE__);
    }

    // Register service with the Binder service manager
    std::string serviceNameStr = serviceObj->getServiceName(); // keep a local copy
    const char* serviceName = serviceNameStr.c_str();

    binder_status_t status = AServiceManager_addService(serviceObj->asBinder().get(), serviceName);
    if (status != STATUS_OK)
    {
        ALOGE("%s:%d: Failed to register service '%s'", __FUNCTION__, __LINE__, serviceName);
        return -1;
    }

    ALOGI("%s:%d: TemperatureService registered as '%s', joining thread pool", __FUNCTION__, __LINE__, serviceName);

    // This blocks the main thread and starts processing incoming binder calls
    ABinderProcess_joinThreadPool();

    ALOGW("%s:%d: Service exited unexpectedly", __FUNCTION__, __LINE__);

    return 0; // should never reach here
}
