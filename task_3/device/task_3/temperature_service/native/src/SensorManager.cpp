#include "SensorManager.h"
#include <utils/Log.h>

#ifdef LOG_TAG
   #undef LOG_TAG
#endif
#define LOG_TAG "temperature_service"

SensorManager::SensorManager() : mLib(nullptr) {}
SensorManager::~SensorManager() = default;

bool SensorManager::initialize()
{
    mLib = ::std::make_unique<SensorLibrary>();

    return true;
}

std::vector<SensorData> SensorManager::getReadings()
{
    if (!mLib)
        return {};
    return mLib->getSensorReadings();
}

bool SensorManager::setVoltage(double voltage)
{
    if (!mLib)
        return false;
    mLib->setCurrentVoltage(voltage);
    return true;
}