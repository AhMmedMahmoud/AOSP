#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <string>
#include <vector>
#include <memory>
#include "SensorLibrary.hpp"

class SensorManager
{
public:
    SensorManager();
    ~SensorManager();

    bool initialize();

    std::vector<SensorData> getReadings();
    bool setVoltage(double voltage);

private:
    std::unique_ptr<SensorLibrary> mLib;
};

#endif