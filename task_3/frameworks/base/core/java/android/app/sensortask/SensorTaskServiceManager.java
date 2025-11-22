package android.app.sensortask;

import android.annotation.FlaggedApi;
import android.annotation.NonNull;
import android.annotation.Nullable;
import android.annotation.SystemService;
import android.content.Context;
import android.os.RemoteException;
import android.util.Log;

import java.util.List;

import android.annotation.SystemApi;

/** @hide */
@SystemService(Context.SENSOR_TASK_SERVICE)
public class SensorTaskServiceManager {
    
    private static final String TAG = "SensorTaskServiceManager";
    private final @NonNull ISensorTaskService mService;

    /** @hide */
    public SensorTaskServiceManager(@NonNull ISensorTaskService service) {
        mService = service;
    }

    /** @hide */
    @Nullable
    public List<SensorReading> getAllSensorReadings() {
        try {
            return mService.getAllSensorReadings();
        } catch (RemoteException e) {
            Log.e(TAG, "SensorTaskServiceManager.java::::  getAllSensorReadings failed", e);
            return null;
        }
    }

    /** @hide */
    public void setCurrentVoltage(double voltage) {
        try {
            mService.setCurrentVoltage(voltage);
        } catch (RemoteException e) {
            Log.e(TAG, "SensorTaskServiceManager.java::::  setCurrentVoltage failed", e);
        }
    }
}
