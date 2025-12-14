# Fake VHAL – Quick Setup Guide

This guide explains how to add two custom properties to the Fake Vehicle HAL:
1) a static read-only property, and 2) a read/write property that updates continuously.
It also shows how to implement them in the Fake VHAL and test them from shell, Android apps,
and native C++ code.

## 1. Add New Test Properties
1. Open `TestVendorProperty.aidl` and define:
   - One static read-only property
   - One read/write property that will change continuously

## 2. Implement Behavior in FakeVehicleHardware.cpp
- In `get()` return correct values for:
  - Read-only property (static)
  - Read/write property (updated periodically)
- In `set()` handle updates only for the writable property.

## 3. Test from Shell
- List available properties:
  `adb shell cmd car_service list-vhal-props`
- Get value:
  `adb shell cmd car_service get-property-value <prop-id>`
- Set value (only for writable property):
  `adb shell cmd car_service set-property-value <prop-id> <value>`

## 4. Access from Android App
- Use `CarPropertyManager` to:
  - Register a listener
  - Read your properties
  - Write to the writable one

## 5. Access from Native C++
- by binding directly to the VHAL AIDL service (IVehicle) and communicating through Binder IPC.
  - Connect to VHAL
  - Read your static property
  - Read and write your dynamic property

