### AOSP Android 16 for Raspberry Pi 4 and Raspberry Pi 5.

***

### How to build (Ubuntu 22.04 LTS):

1. Establish [Android build environment](https://source.android.com/docs/setup/start/requirements).

2. Install additional packages:

```
sudo apt-get install dosfstools e2fsprogs fdisk kpartx mtools rsync
```

3. Initialize repo:

```
repo init -u https://android.googlesource.com/platform/manifest -b android-16.0.0_r1
curl -o .repo/local_manifests/manifest_brcm_rpi.xml -L https://raw.githubusercontent.com/raspberry-vanilla/android_local_manifest/android-16.0/manifest_brcm_rpi.xml --create-dirs
```

4. Sync source code:

```
repo sync -j8
```

5. Setup Android build environment:

```
. build/envsetup.sh
```

6. Select the device (`rpi4` or `rpi5`) and build target (tablet UI, `tv` for Android TV, or `car` for Android Automotive):

```
lunch aosp_rpi4-bp2a-userdebug
```
```
lunch aosp_rpi4_tv-bp2a-userdebug
```
```
lunch aosp_rpi4_car-bp2a-userdebug
```
```
lunch aosp_rpi5-bp2a-userdebug
```
```
lunch aosp_rpi5_tv-bp2a-userdebug
```
```
lunch aosp_rpi5_car-bp2a-userdebug
```

7. Compile:

```
make bootimage systemimage vendorimage -j$(nproc)
```

8. Make flashable image for the device (`rpi4` or `rpi5`):

```
./rpi4-mkimg.sh
```
```
./rpi5-mkimg.sh
```

***
