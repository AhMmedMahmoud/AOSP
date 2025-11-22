package android.app.sensortask;

import android.app.sensortask.TemperatureReading;

/** @hide */
@VintfStability 
interface IReadingsCallback {
    void onResult(in TemperatureReading[] readings);
}
