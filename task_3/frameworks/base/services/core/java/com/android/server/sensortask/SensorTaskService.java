// package com.android.server.sensortask;

// import android.app.sensortask.ISensorTaskService;
// import android.app.sensortask.SensorReading;
// import android.content.Context;
// import android.os.RemoteException;
// import android.util.Slog;
// import com.android.server.SystemService;
// import java.util.ArrayList;
// import java.util.List;

// public final class SensorTaskService extends SystemService {

//     private static final String TAG = "SensorTaskService";

//     // Load native library
//     static {
//         System.loadLibrary("sensor_task_jni");
//     }

//     // Native method declarations
//     private native boolean nativeInit();
//     private native void nativeSetCurrentVoltage(double voltage);
//     private native SensorReading[] nativeGetAllSensorReadings();

//     // Binder implementation
//     private final ISensorTaskService.Stub mBinder = new BinderImpl();

//     public SensorTaskService(Context context) {
//         super(context);
//     }

//     @Override
//     public void onStart() {
//         Slog.i(TAG, "SensorTaskService.java::::   Starting SensorTaskService");

//         // Initialize native layer
//         if (!nativeInit()) {
//             Slog.e(TAG, "SensorTaskService.java::::   Failed to initialize native layer");
//             return;
//         }

//         // Publish binder to system
//         publishBinderService(Context.SENSOR_TASK_SERVICE, mBinder);

//         Slog.i(TAG, "SensorTaskService.java::::   SensorTaskService published successfully");
//     }

//     /** Inner class implementing the AIDL interface */
//     private class BinderImpl extends ISensorTaskService.Stub {

//         @Override
//         public List<SensorReading> getAllSensorReadings() throws RemoteException {
//             try {
//                 SensorReading[] readings = nativeGetAllSensorReadings();
                
//                 List<SensorReading> result = new ArrayList<>();
//                 if (readings != null) {
//                     for (SensorReading reading : readings) {
//                         result.add(reading);
//                     }
//                 }
                
//                 return result;
//             } catch (Exception e) {
//                 Slog.e(TAG, "SensorTaskService.java::::   Error getting sensor readings", e);
//                 throw new RemoteException("Failed to get sensor readings: " + e.getMessage());
//             }
//         }

//         @Override
//         public void setCurrentVoltage(double voltage_value) throws RemoteException {
//             try {
//                 nativeSetCurrentVoltage(voltage_value);
//             } catch (Exception e) {
//                 Slog.e(TAG, "SensorTaskService.java::::   Error setting voltage", e);
//                 throw new RemoteException("Failed to set voltage: " + e.getMessage());
//             }
//         }
//     }
// }


package com.android.server.sensortask;

import android.app.sensortask.ISensorTaskService;
import android.app.sensortask.SensorReading;
import android.content.Context;
import android.os.RemoteException;
import android.util.Slog;

import com.android.server.SystemService;

import android.app.sensortask.TemperatureReading;

import java.util.ArrayList;
import java.util.List;

public final class SensorTaskService extends SystemService {

    private static final String TAG = "SensorTaskService";
    private TemperatureServiceClient mTempClient;
    private final ISensorTaskService.Stub mBinder = new BinderImpl();

    public SensorTaskService(Context context) {
        super(context);
    }

    @Override
    public void onStart() {
        Slog.i(TAG, "Starting SensorTaskService");

        // Initialize temperature service client
        mTempClient = new TemperatureServiceClient();
        if (!mTempClient.init()) {
            Slog.e(TAG, "Failed to initialize temperature client");
            return;
        }

        // Publish binder
        publishBinderService(Context.SENSOR_TASK_SERVICE, mBinder);
        Slog.i(TAG, "SensorTaskService published successfully");
    }

    private class BinderImpl extends ISensorTaskService.Stub {

        @Override
        public List<SensorReading> getAllSensorReadings() throws RemoteException {
            try {
                List<TemperatureReading> tempReadings = mTempClient.getAllReadings();
                
                List<SensorReading> result = new ArrayList<>();
                if (tempReadings != null) {
                    for (TemperatureReading tr : tempReadings) {
                        SensorReading sr = new SensorReading();
                        sr.value = tr.value;
                        sr.timestamp = tr.timestamp;
                        result.add(sr);
                    }
                }
                
                return result;
            } catch (Exception e) {
                Slog.e(TAG, "Error getting sensor readings", e);
                throw new RemoteException("Failed to get sensor readings: " + e.getMessage());
            }
        }

        @Override
        public void setCurrentVoltage(double voltage_value) throws RemoteException {
            try {
                mTempClient.setVoltage(voltage_value);
            } catch (Exception e) {
                Slog.e(TAG, "Error setting voltage", e);
                throw new RemoteException("Failed to set voltage: " + e.getMessage());
            }
        }
    }
}