package android.app.sensortask;

import android.app.sensortask.TemperatureReading;
import android.app.sensortask.IReadingsCallback;

/** @hide */
@VintfStability 
interface ITemperatureService {
    List<TemperatureReading> getAllTemperatureReadings();
    void setCurrentVoltage(double voltage_value);
    void getAllReadingAsync(in IReadingsCallback callback);
    void registerCallback(in IReadingsCallback callback);
    void unregisterCallback(in IReadingsCallback callback);
}