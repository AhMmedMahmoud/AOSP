#include <jni.h>
#include <android/log.h>
#include <aidl/android/app/sensortask/ITemperatureService.h>
#include <aidl/android/app/sensortask/TemperatureReading.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <memory>
#include <vector>

#ifndef LOG_TAG
#define LOG_TAG "SensorTaskJNI"
#endif

#ifndef ALOGE
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

#ifndef ALOGW
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#endif

#ifndef ALOGI
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#endif

namespace android {    
    using namespace aidl::android::app::sensortask;

    // Global reference to the Temperature Service
    static std::shared_ptr<ITemperatureService> gTemperatureService = nullptr;

    // Initialize connection to Temperature Service
    extern "C" JNIEXPORT jboolean JNICALL
    Java_com_android_server_sensortask_SensorTaskService_nativeInit(JNIEnv* env, jobject /* this */) {
        ALOGI("com_android_server_sensortask_SensorTaskService.cpp:::: nativeInit called");

        if (gTemperatureService != nullptr) {
            return JNI_TRUE;
        }

        const char* serviceName = "android.app.sensortask.ITemperatureService/default";
        
        // Get binder
        auto binder = ndk::SpAIBinder(AServiceManager_getService(serviceName));
        if (binder.get() == nullptr) {
            ALOGE("com_android_server_sensortask_SensorTaskService.cpp:::: sensor_task_jni:::: Failed to get service: %s", serviceName);
            return JNI_FALSE;
        }

        gTemperatureService = ITemperatureService::fromBinder(binder);
        if (gTemperatureService == nullptr) {
            ALOGE("com_android_server_sensortask_SensorTaskService.cpp:::: Failed to cast binder to ITemperatureService");
            return JNI_FALSE;
        }

        ALOGI("com_android_server_sensortask_SensorTaskService.cpp:::: Successfully connected to Temperature Service");
        return JNI_TRUE;
    }

    extern "C" JNIEXPORT void JNICALL
    Java_com_android_server_sensortask_SensorTaskService_nativeSetCurrentVoltage(JNIEnv* env, jobject /* this */, jdouble voltage) {
        if (gTemperatureService == nullptr) {
            ALOGE("com_android_server_sensortask_SensorTaskService.cpp:::: Temperature service not initialized");
            return;
        }

        auto status = gTemperatureService->setCurrentVoltage(static_cast<double>(voltage));
        if (!status.isOk()) {
            ALOGE("com_android_server_sensortask_SensorTaskService.cpp:::: setCurrentVoltage failed: %s", status.getDescription().c_str());
        } else {
            ALOGI("com_android_server_sensortask_SensorTaskService.cpp:::: setCurrentVoltage(%.2f) called successfully", voltage);
        }
    }

    extern "C" JNIEXPORT jobjectArray JNICALL
    Java_com_android_server_sensortask_SensorTaskService_nativeGetAllSensorReadings( JNIEnv* env, jobject /* this */) {
        if (gTemperatureService == nullptr) {
            ALOGE("com_android_server_sensortask_SensorTaskService.cpp:::: Temperature service not initialized");
            return nullptr;
        }

        std::vector<TemperatureReading> readings;
        auto status = gTemperatureService->getAllTemperatureReadings(&readings);
        
        if (!status.isOk()) {
            ALOGE("com_android_server_sensortask_SensorTaskService.cpp:::: getAllTemperatureReadings failed: %s", status.getDescription().c_str());
            return nullptr;
        }

        // Find the SensorReading class
        jclass sensorReadingClass = env->FindClass("android/app/sensortask/SensorReading");
        if (sensorReadingClass == nullptr) {
            ALOGE("com_android_server_sensortask_SensorTaskService.cpp:::: Failed to find SensorReading class");
            return nullptr;
        }

        // Get the constructor
        jmethodID constructor = env->GetMethodID(sensorReadingClass, "<init>", "()V");
        if (constructor == nullptr) {
            ALOGE("com_android_server_sensortask_SensorTaskService.cpp:::: Failed to find SensorReading constructor");
            return nullptr;
        }

        // Get field IDs
        jfieldID valueField = env->GetFieldID(sensorReadingClass, "value", "D");
        jfieldID timestampField = env->GetFieldID(sensorReadingClass, "timestamp", "J");
        
        if (valueField == nullptr || timestampField == nullptr) {
            ALOGE("com_android_server_sensortask_SensorTaskService.cpp:::: Failed to find SensorReading fields");
            return nullptr;
        }

        // Create array of SensorReading objects
        jobjectArray result = env->NewObjectArray(readings.size(), sensorReadingClass, nullptr);
        if (result == nullptr) {
            ALOGE("com_android_server_sensortask_SensorTaskService.cpp:::: Failed to create SensorReading array");
            return nullptr;
        }

        // Populate the array
        for (size_t i = 0; i < readings.size(); ++i) {
            jobject sensorReading = env->NewObject(sensorReadingClass, constructor);
            if (sensorReading == nullptr) {
                ALOGE("com_android_server_sensortask_SensorTaskService.cpp:::: Failed to create SensorReading object at index %zu", i);
                continue;
            }

            env->SetDoubleField(sensorReading, valueField, readings[i].value);
            env->SetLongField(sensorReading, timestampField, readings[i].timestamp);
            
            env->SetObjectArrayElement(result, i, sensorReading);
            env->DeleteLocalRef(sensorReading);
        }

        ALOGI("com_android_server_sensortask_SensorTaskService.cpp:::: Returning %zu sensor readings", readings.size());
        return result;
    }

    int register_com_android_server_sensortask_SensorTaskService(JNIEnv* env) {
        ALOGI("com_android_server_sensortask_SensorTaskService.cpp:::: Registering SensorTaskService JNI methods");
        
        const JNINativeMethod methods[] = {
            {"nativeInit", "()Z", (void*)Java_com_android_server_sensortask_SensorTaskService_nativeInit},
            {"nativeSetCurrentVoltage", "(D)V", (void*)Java_com_android_server_sensortask_SensorTaskService_nativeSetCurrentVoltage},
            {"nativeGetAllSensorReadings", "()[Landroid/app/sensortask/SensorReading;", (void*)Java_com_android_server_sensortask_SensorTaskService_nativeGetAllSensorReadings},
        };
        
        jclass clazz = env->FindClass("com/android/server/sensortask/SensorTaskService");
        if (clazz == nullptr) {
            ALOGE("com_android_server_sensortask_SensorTaskService.cpp:::: Failed to find SensorTaskService class");
            return JNI_ERR;
        }
        
        int result = env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0]));
        if (result < 0) {
            ALOGE("com_android_server_sensortask_SensorTaskService.cpp:::: Failed to register native methods");
            return JNI_ERR;
        }
        
        ALOGI("com_android_server_sensortask_SensorTaskService.cpp:::: Successfully registered SensorTaskService JNI methods");
        return JNI_VERSION_1_4;
    }
}