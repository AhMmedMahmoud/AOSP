package com.example.sensortasktest;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import android.os.IBinder;
import android.os.ServiceManager;
import android.content.Context;

import android.app.sensortask.ISensorTaskService;
import android.app.sensortask.SensorReading;

import java.util.List;

public class MainActivity extends Activity {

    private static final String TAG = "SensorTaskTest";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        TextView tv = new TextView(this);
        tv.setText("Running SensorTask Test…");
        setContentView(tv);

        new Thread(() -> runTest(tv)).start();
    }

    private void runTest(TextView tv) {
        try {
            Log.i(TAG, "packages/apps/SensorTaskTestApp/src/com/example/sensortasktest/MainActivity.java:::::   Getting SensorTaskService binder...");

            // Get binder directly from ServiceManager
            IBinder binder = ServiceManager.getService(Context.SENSOR_TASK_SERVICE);

            if (binder == null) {
                update(tv, "packages/apps/SensorTaskTestApp/src/com/example/sensortasktest/MainActivity.java:::::   ERROR: SensorTaskService binder is NULL");
                return;
            }

            ISensorTaskService service = ISensorTaskService.Stub.asInterface(binder);

            if (service == null) {
                update(tv, "packages/apps/SensorTaskTestApp/src/com/example/sensortasktest/MainActivity.java:::::  ERROR: SensorTaskService interface is NULL");
                return;
            }

            update(tv, "packages/apps/SensorTaskTestApp/src/com/example/sensortasktest/MainActivity.java:::::   Connected to SensorTaskService");

            // --- TEST 1: set voltage
            try {
                service.setCurrentVoltage(4.20);
                update(tv, "packages/apps/SensorTaskTestApp/src/com/example/sensortasktest/MainActivity.java:::::   Voltage set to 4.20V successfully");
            } catch (Exception e) {
                update(tv, "packages/apps/SensorTaskTestApp/src/com/example/sensortasktest/MainActivity.java:::::   Failed setCurrentVoltage: " + e);
            }

            // --- TEST 2: get readings
            try {
                List<SensorReading> readings = service.getAllSensorReadings();

                if (readings == null) {
                    update(tv, "packages/apps/SensorTaskTestApp/src/com/example/sensortasktest/MainActivity.java:::::   getAllSensorReadings returned NULL");
                } else {
                    StringBuilder sb = new StringBuilder();
                    sb.append("Readings count: ").append(readings.size()).append("\n\n");

                    for (SensorReading r : readings) {
                        sb.append("Value: ").append(r.value)
                                .append("  Timestamp: ").append(r.timestamp)
                                .append("\n");
                    }

                    update(tv, sb.toString());
                }
            } catch (Exception e) {
                update(tv, "packages/apps/SensorTaskTestApp/src/com/example/sensortasktest/MainActivity.java:::::   Failed getAllSensorReadings: " + e);
            }

        } catch (Exception ex) {
            update(tv, "packages/apps/SensorTaskTestApp/src/com/example/sensortasktest/MainActivity.java:::::   Exception: " + ex.toString());
        }
    }

    private void update(TextView tv, String text) {
        runOnUiThread(() -> tv.setText(text));
        Log.i(TAG, text);
    }
}
