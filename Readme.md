# AOSP For Default Targets Supported by Google

## Install all the required packages
```bash
sudo apt-get install openjdk-8-jdk android-tools-adb bc bison build-essential curl flex g++-multilib gcc-multilib gnupg gperf imagemagick lib32ncurses5-dev lib32readline-dev lib32z1-dev liblz4-tool libncurses5-dev libsdl1.2-dev libssl-dev libwxgtk3.0-gtk3-dev libxml2 libxml2-utils lzop pngcrush rsync schedtool squashfs-tools xsltproc yasm zip zlib1g-dev git-core python3.8 libncurses5
```

### Download & give path for git repo
```bash
sudo apt-get update
mkdir ~/bin
PATH=~/bin:$PATH
curl https://storage.googleapis.com/git-repo-downloads/repo > ~/bin/repo
chmod a+x ~/bin/repo
```

### Initialise AOSP-git repo
```bash
mkdir aosp14
cd aosp14
repo init -u https://android.googlesource.com/platform/manifest -b android-14.0.0_r3
repo sync -j16
```

### Build AOSP
```bash
source build/envsetup.sh
lunch
type 14 for aosp_cf_x86_64_auto
make -j16
```
