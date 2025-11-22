package android.app.sensortask;

import android.app.sensortask.SensorReading;

/** @hide */
interface ISensorTaskService {
    List<SensorReading> getAllSensorReadings();
    void setCurrentVoltage(double voltage);
}