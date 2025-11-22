package android.app.sensortask;

import android.app.sensortask.TemperatureReading;

/** @hide */
interface ITemperatureService {
    List<TemperatureReading> getAllTemperatureReadings();
    void setCurrentVoltage(double voltage_value);
}