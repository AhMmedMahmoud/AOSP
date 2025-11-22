#include "CallbackManager.h"
#include <iostream>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "temperature_service"

void CallbackManager::registerCallback(const Callback &cb)
{
    if (!cb) return;
    
    std::lock_guard<std::mutex> lk(mMutex);
    for (auto &w : mCallbacks)
    {
        auto s = w.lock();
        if (s && s == cb)
            return;
    }
    mCallbacks.emplace_back(cb);
}

void CallbackManager::unregisterCallback(const Callback &cb)
{
    std::lock_guard<std::mutex> lk(mMutex);
    mCallbacks.erase(
        std::remove_if(mCallbacks.begin(), mCallbacks.end(),
                       [&](const std::weak_ptr<aidl::android::app::sensortask::IReadingsCallback> &w)
                       {
                           auto s = w.lock();
                           return (!s) || (s == cb);
                       }),
        mCallbacks.end());
}

void CallbackManager::notifyAll(const std::vector<aidl::android::app::sensortask::TemperatureReading> &readings)
{
    std::vector<Callback> alive;
    {
        std::lock_guard<std::mutex> lk(mMutex);
        for (auto &w : mCallbacks)
        {
            if (auto s = w.lock())
                alive.push_back(s);
        }
    }

    for (auto &cb : alive)
    {
        if (auto s = cb)
        {
            s->onResult(readings);
        }
    }
}
