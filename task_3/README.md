# Java System Service Communicating with Native C++ Service

This project implements an Android **Java System Service** that interacts with a **native (C++) system service** responsible for temperature and sensor readings. Communication is handled via **AIDL interfaces**.  
The Java service acts as a client to the native layer and exposes a framework API for applications through Java-side AIDL interfaces and a Manager class.

# Architecture 
<img width="623" height="221" alt="image" src="https://github.com/user-attachments/assets/dad8511a-8576-4400-91f5-6a3d88cd99a5" />

# Extra Work
a JNI layer is implemented to map Java service calls directly to the corresponding C++ service calls.

------------------------------------------------------------
📁 Folder Structure
------------------------------------------------------------
``` bash
.
├── device
│   └── task_3
│       ├── sensor_lib
│       │   ├── Android.bp
│       │   ├── inc
│       │   │   └── SensorLibrary.hpp
│       │   └── src
│       │       └── SensorLibrary.cpp
│       ├── temperature_service
│       │   ├── Android.bp
│       │   ├── interfaces/android/app/sensortask
│       │   │   ├── IReadingsCallback.aidl
│       │   │   ├── ITemperatureService.aidl
│       │   │   └── TemperatureReading.aidl
│       │   ├── native
│       │   │   ├── inc
│       │   │   │   ├── CallbackManager.h
│       │   │   │   ├── SensorManager.h
│       │   │   │   └── TemperatureService.h
│       │   │   └── src
│       │   │       ├── CallbackManager.cpp
│       │   │       ├── main.cpp
│       │   │       ├── SensorManager.cpp
│       │   │       └── TemperatureService.cpp
│       │   └── temperature_service.rc
│       ├── temperature_service_user_app
│       │   ├── Android.bp
│       │   └── main.cpp
│       └── to_append
│           ├── device/brcm/rpi5
│           │   ├── aosp_rpi5_car.mk
│           │   ├── manifest.xml
│           │   └── sepolicy/service_contexts
│           └── system/sepolicy/vendor
│               ├── domain.te
│               ├── file_contexts
│               ├── service.te
│               └── temperature_service.te
├── frameworks
│   └── base
│       ├── core/java/android/app/sensortask
│       │   ├── ISensorTaskService.aidl
│       │   ├── ITemperatureService.aidl
│       │   ├── SensorReading.aidl
│       │   ├── SensorTaskServiceManager.java
│       │   └── TemperatureReading.aidl
│       ├── services/core/java/com/android/server/sensortask
│       │   ├── SensorTaskService.java
│       │   └── TemperatureServiceClient.java
│       ├── services/core/jni
│       │   ├── Android.bp
│       │   ├── com_android_server_sensortask_SensorTaskService.cpp
│       │   └── onload.cpp
│       └── services/java/com/android/server/SystemServer.java
└── packages/apps/SensorTaskTestApp
    ├── Android.bp
    ├── AndroidManifest.xml
    └── src/com/example/sensortasktest/MainActivity.java
```

------------------------------------------------------------
⚠️ Common Issues & Fixes
------------------------------------------------------------

Issue 1:
E BpBinder: Cannot do a user transaction on a vendor stability binder (temperature.service.api.ITemperatureService)

Reason:
Java service runs in the system partition, but the native service is in the vendor partition.  
The AIDL interface was marked as `unstable`, which cannot cross system/vendor boundaries.

Takeaway:
Mark all AIDL types with:
    @VintfStability
This makes the interface stable across vendor ↔ system processes.

------------------------------------------------------------

Issue 2:
Could not find temperature.service.api.ITemperatureService/default in the VINTF manifest

Reason:
Service instance not declared in vendor manifest.

Takeaway:
Add the AIDL HAL entry to vendor manifest:
``` bash
<manifest version="1.0" type="device">
    <hal format="aidl">
        <name>android.app.sensortask</name>
        <version>2</version>
        <fqname>ITemperatureService/default</fqname>
    </hal>
</manifest>
```
------------------------------------------------------------

Issue 3:
**** enforceInterface() expected 'temperature.service.api.ITemperatureService'
but read 'android.app.sensortask.ITemperatureService'

Reason:
Native service uses package `temperature.service.api`  
Java service uses package `android.app.sensortask`  
AIDL package mismatch → Binder rejects transactions.

Takeaway:
Both C++ and Java must share the EXACT same AIDL package name.

------------------------------------------------------------

Issue 4:
EX_TRANSACTION_FAILED: 'BAD_TYPE'

Reason:
Same as above — mismatched AIDL packages.

Takeaway:
Use a unified package, e.g.:
    package android.app.sensortask;

------------------------------------------------------------

Issue 5:
Java compilation error:
package aidl.temperature.service.api does not exist

Reason:
Framework Java layer does not have visibility to generated AIDL classes.

Takeaway:
Expose AIDL via Android.bp (include_dirs), OR place native AIDL next to Java AIDL.

------------------------------------------------------------
✔ Summary
------------------------------------------------------------

- Use @VintfStability for cross-partition (system ↔ vendor) AIDL.
- Declare the service instance in vendor VINTF manifest.
- Both Java and C++ AIDL must share the same package.
- Give framework services access to generated AIDL classes.
- BAD_TYPE and enforceInterface errors almost always mean AIDL mismatch.

