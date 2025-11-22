package com.android.server.sensortask;

import android.os.IBinder;
import android.os.ServiceManager;
import android.os.RemoteException;
import android.util.Slog;

import android.app.sensortask.ITemperatureService;
import android.app.sensortask.TemperatureReading;

import java.util.List;

public class TemperatureServiceClient {
    private static final String TAG = "TemperatureServiceClient";
    private static final String SERVICE_NAME = "android.app.sensortask.ITemperatureService/default";
    
    private ITemperatureService mService;
    
    public boolean init() {
        try {
            IBinder binder = ServiceManager.getService(SERVICE_NAME);
            if (binder == null) {
                Slog.e(TAG, "Failed to get service: " + SERVICE_NAME);
                return false;
            }
            
            mService = ITemperatureService.Stub.asInterface(binder);
            if (mService == null) {
                Slog.e(TAG, "Failed to cast to ITemperatureService");
                return false;
            }
            
            Slog.i(TAG, "Successfully connected to temperature service");
            return true;
        } catch (Exception e) {
            Slog.e(TAG, "Failed to connect to temperature service", e);
            return false;
        }
    }
    
    public List<TemperatureReading> getAllReadings() throws RemoteException {
        if (mService == null) {
            throw new IllegalStateException("Service not initialized");
        }
        return mService.getAllTemperatureReadings();
    }
    
    public void setVoltage(double voltage) throws RemoteException {
        if (mService == null) {
            throw new IllegalStateException("Service not initialized");
        }
        mService.setCurrentVoltage(voltage);
    }
}