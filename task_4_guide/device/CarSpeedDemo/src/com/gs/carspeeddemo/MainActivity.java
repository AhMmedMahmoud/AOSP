package com.gs.carspeeddemo;

import androidx.appcompat.app.AppCompatActivity;

import android.car.Car;
import android.car.VehiclePropertyIds;
import android.car.hardware.CarPropertyValue;
import android.car.hardware.property.CarPropertyManager;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "CarSpeedDemo";
    private TextView mTvSpeed;
    
    private Car mCar;
    CarPropertyManager mCarPropertyManager;

    String[] perms = {"android.car.permission.CAR_SPEED"};
    int permsRequestCode = 200;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        mTvSpeed = findViewById(R.id.tv_speed);

        requestPermissions(perms, permsRequestCode);
    }

    CarPropertyManager.CarPropertyEventCallback mCallBack = new CarPropertyManager.CarPropertyEventCallback() {
        @Override
        public void onChangeEvent(CarPropertyValue carPropertyValue) {
            Log.d(TAG, "onChangeEvent Property: " + carPropertyValue.getPropertyId());
            if (carPropertyValue.getPropertyId() == 0x11600207) {
                float value = (Float) carPropertyValue.getValue();
                mTvSpeed.setText("Speed: " + Math.round(value));
            }
        }

        @Override
        public void onErrorEvent(int i, int i1) {
            Log.e(TAG, "onErrorEvent: " + i);
        }
    };

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        if(requestCode==200 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            Log.d(TAG, "onRequestPermissionsResult: " + requestCode);
            mCar = Car.createCar(this);
            mCarPropertyManager = (CarPropertyManager) mCar.getCarManager(Car.PROPERTY_SERVICE);
            mCarPropertyManager.registerCallback(mCallBack, VehiclePropertyIds.PERF_VEHICLE_SPEED, CarPropertyManager.SENSOR_RATE_NORMAL);

            Log.d(TAG, "onRequestPermissionsResult: isConnected " + mCar.isConnected());
        }
    }

    @Override
    protected void onStop() {
        super.onStop();

        mCarPropertyManager.unregisterCallback(mCallBack);
        mCar.disconnect();
    }
}