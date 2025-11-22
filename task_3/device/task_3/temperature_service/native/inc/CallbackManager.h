#ifndef CALLBACK_MANAGER_H
#define CALLBACK_MANAGER_H

#include <vector>
#include <memory>
#include <mutex>
#include <algorithm>
#include <aidl/android/app/sensortask/IReadingsCallback.h>
#include <aidl/android/app/sensortask/TemperatureReading.h>

class CallbackManager
{
public:
    using Callback = std::shared_ptr<aidl::android::app::sensortask::IReadingsCallback>;

    void registerCallback(const Callback &cb);
    void unregisterCallback(const Callback &cb);
    void notifyAll(const std::vector<aidl::android::app::sensortask::TemperatureReading> &readings);

private:
    std::mutex mMutex;
    std::vector<std::weak_ptr<aidl::android::app::sensortask::IReadingsCallback>> mCallbacks;
};

#endif