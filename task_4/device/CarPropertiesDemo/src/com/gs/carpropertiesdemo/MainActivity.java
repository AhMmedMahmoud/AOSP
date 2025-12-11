package com.gs.carpropertiesdemo;

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

    private static final String TAG = "CarPropertiesDemo";
    private TextView mTvStatic;
    private TextView mTvDynamic;
    
    private Car mCar;
    CarPropertyManager mCarPropertyManager;

    private static final int STATIC_PROP = 0x21402b00;
    private static final int DYNAMIC_PROP = 0x21402b01;

    String[] perms = {
        "android.car.permission.CAR_SPEED",
        "android.car.permission.CAR_VENDOR_EXTENSION"
    };
    int permsRequestCode = 200;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        mTvStatic = findViewById(R.id.tv_static);
        mTvDynamic = findViewById(R.id.tv_dynamic);

        requestPermissions(perms, permsRequestCode);
    }

    CarPropertyManager.CarPropertyEventCallback mCallBack = new CarPropertyManager.CarPropertyEventCallback() {
        @Override
        public void onChangeEvent(CarPropertyValue carPropertyValue) {
            int propId = carPropertyValue.getPropertyId();
            Log.d(TAG, "onChangeEvent: " + propId);

            if (propId == STATIC_PROP) {
                int value = (Integer) carPropertyValue.getValue();
                mTvStatic.setText("Static: " + value);
            } else if (propId == DYNAMIC_PROP) {
                int value = (Integer) carPropertyValue.getValue();
                mTvDynamic.setText("Dynamic: " + value);
            }
        }

        @Override
        public void onErrorEvent(int propId, int zone) {
            Log.e(TAG, "onErrorEvent: " + propId);
        }
    };


    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        if(requestCode==200 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            Log.d(TAG, "onRequestPermissionsResult: " + requestCode);
            mCar = Car.createCar(this);
            mCarPropertyManager = (CarPropertyManager) mCar.getCarManager(Car.PROPERTY_SERVICE);
            mCarPropertyManager.registerCallback(mCallBack, STATIC_PROP, CarPropertyManager.SENSOR_RATE_ONCHANGE);
            mCarPropertyManager.registerCallback(mCallBack, DYNAMIC_PROP, CarPropertyManager.SENSOR_RATE_NORMAL);

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