# Simple Calculator (AOSP Vendor Example)

This project demonstrates adding a native shared library and a binary to the AOSP vendor build for Raspberry Pi 5.

## What’s Included
- `math_lib` shared library (provides `add(a, b)`)
- `simple_calculator` binary (uses the library)
- `aosp_rpi5_car.mk` update (adds app to vendor image)
- `setup.sh` (copies required files into AOSP tree)

## How to Integrate into AOSP
```
cd device/task_1
./setup.sh /path/to/AOSP
```

## Build
```
source build/envsetup.sh
lunch aosp_rpi5_car-bp2a-userdebug
m -j8 simple_calculator
m -j8
```

## Flash the target 

## Run on device
``` 
adb shell 
ls simple_calculator
simple_calculator
```
Enter two numbers when prompted, and it prints the sum.