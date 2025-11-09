# Temperature Service (AIDL HAL) for Android (Raspberry Pi 5)

This project provides a vendor-side AIDL-based temperature service for Raspberry Pi 5, along with a test client application. The service reads a voltage value, converts it into a temperature value, stores recent readings, and provides both synchronous and asynchronous access to the data.

## Components

| Path | Description |
|------|-------------|
| temperature_service/ | Main AIDL service implementation (runs under /vendor/bin) |
| temperature_service_user_app/ | User-space client application for testing the service |
| sensor_lib/ | Simple sensor library used for reading and converting data |
| to_append/ | Contains integration files for AOSP (manifest, SELinux, product configs, etc.) |
| setup.sh | Script to copy the integration files into an existing AOSP source tree |

## How to Integrate into AOSP
```
cd device/task_2  
./setup.sh /path/to/aosp
```

## Build
```
source build/envsetup.sh  
lunch aosp_rpi5_car-bp2a-userdebug
m -j8 temperature_service temperature_service_user_app
m -j8
```

## Flash the target 
The service starts automatically on boot.

## Running the Test Client
```
adb shell  
temperature_service_user_app
```

Expected output includes:
- Voltage readings
- Calculated temperature
- List of recently stored readings

## Logging
```
adb logcat | grep temperature_service
```

## Service Name

Clients must request the service using:
```
temperature.service.api.ITemperatureService/default
```
This string must be passed to AServiceManager_getService.
