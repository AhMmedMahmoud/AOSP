#ifndef CALLBACK_MANAGER_H
#define CALLBACK_MANAGER_H

#include <vector>
#include <memory>
#include <mutex>
#include <algorithm>
#include <aidl/temperature/service/api/IReadingsCallback.h>
#include <aidl/temperature/service/api/TemperatureReading.h>

class CallbackManager
{
public:
    using Callback = std::shared_ptr<aidl::temperature::service::api::IReadingsCallback>;

    void registerCallback(const Callback &cb);
    void unregisterCallback(const Callback &cb);
    void notifyAll(const std::vector<aidl::temperature::service::api::TemperatureReading> &readings);

private:
    std::mutex mMutex;
    std::vector<std::weak_ptr<aidl::temperature::service::api::IReadingsCallback>> mCallbacks;
};

#endif