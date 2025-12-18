package temperature.service.api;

import temperature.service.api.TemperatureReading;

interface IReadingsCallback {
    void onResult(in TemperatureReading[] readings);
}
