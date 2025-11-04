#ifndef SENSOR_LIB_H
#define SENSOR_LIB_H

#include <vector>
#include <cstdint>
#include <mutex>


struct SensorData {
    double temperature;
    int64_t timestamp; 
};


class SensorLibrary {
    private:
        std::vector<SensorData> mReadings;
        std::mutex mMutex;

    public:
        SensorLibrary();
    
        std::vector<SensorData> getSensorReadings();

        void setCurrentVoltage(double voltage_value);

        ~SensorLibrary();
        SensorLibrary(const SensorLibrary&) = delete;
        SensorLibrary& operator=(const SensorLibrary&) = delete;
};

#endif