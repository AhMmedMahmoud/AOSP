#include "SensorLibrary.hpp"
#include <chrono>

SensorLibrary::SensorLibrary() {}

SensorLibrary::~SensorLibrary() {}

std::vector<SensorData> SensorLibrary::getSensorReadings()
{
    std::lock_guard<std::mutex> lk(mMutex);
    return mReadings; 
}

void SensorLibrary::setCurrentVoltage(double voltage_value)
{
    double temp = 9.48 * voltage_value + 1.5;
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
                   .count();

    SensorData d{temp, now};

    std::lock_guard<std::mutex> lk(mMutex);
    
    mReadings.push_back(d);
    if (mReadings.size() > 100)
        mReadings.erase(mReadings.begin());
}