package temperature.service.api;

import temperature.service.api.TemperatureReading;
import temperature.service.api.IReadingsCallback;

interface ITemperatureService {
    List<TemperatureReading> getAllTemperatureReadings();
    void setCurrentVoltage(double voltage_value);
    void getAllReadingAsync(in IReadingsCallback callback);
    void registerCallback(in IReadingsCallback callback);
    void unregisterCallback(in IReadingsCallback callback);
}