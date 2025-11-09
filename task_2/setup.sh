#!/bin/bash
set -e

AOSP_ROOT="$1"

if [ -z "$AOSP_ROOT" ]; then
    echo "Usage: $0 <AOSP_ROOT_PATH>"
    exit 1
fi

echo "AOSP ROOT Path: $AOSP_ROOT"

cp -r to_append/system/sepolicy/vendor/* "$AOSP_ROOT/system/sepolicy/vendor/"
cp -r to_append/device/brcm/rpi5/* "$AOSP_ROOT/device/brcm/rpi5/"
cp -r to_append/device/brcm/rpi5/sepolicy/* "$AOSP_ROOT/device/brcm/rpi5/sepolicy/"
