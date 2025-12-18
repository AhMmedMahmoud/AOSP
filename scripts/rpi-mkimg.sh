#!/bin/bash

exit_with_error() {
  echo $@
  exit 1
}

# path to directory contains boot.img, system.img and vendor.img
IMAGES_PATH=$1
IMGNAME="final_image.img"
IMGSIZE=15360000000

# check if images path contains boot, system and vendor images or not
for PARTITION in "boot" "system" "vendor"; do
  if [ ! -f ${IMAGES_PATH}/${PARTITION}.img ]; then
    exit_with_error "Partition image not found."
  fi
done

rm -rf ${IMAGES_PATH}/${IMGNAME}
echo "Creating image file ${IMAGES_PATH}/${IMGNAME}..."
sudo fallocate -l ${IMGSIZE} ${IMAGES_PATH}/${IMGNAME}
sync

echo "Creating partitions..."
(
echo o
echo n
echo p
echo 1
echo
echo +128M
echo n
echo p
echo 2
echo
echo +2560M
echo n
echo p
echo 3
echo
echo +256M
echo n
echo p
echo
echo
echo t
echo 1
echo c
echo a
echo 1
echo w
) | sudo fdisk ${IMAGES_PATH}/${IMGNAME}
sync

LOOPDEV=$(sudo kpartx -av ${IMAGES_PATH}/${IMGNAME} | awk 'NR==1{ sub(/p[0-9]$/, "", $3); print $3 }')
if [ -z ${LOOPDEV} ]; then
  exit_with_error "Unable to find loop device!"
fi
echo "Image mounted as /dev/${LOOPDEV}"
sleep 1

echo "Copying boot..."
sudo dd if=${IMAGES_PATH}/boot.img of=/dev/mapper/${LOOPDEV}p1 bs=1M

echo "Copying system..."
sudo dd if=${IMAGES_PATH}/system.img of=/dev/mapper/${LOOPDEV}p2 bs=1M

echo "Copying vendor..."
sudo dd if=${IMAGES_PATH}/vendor.img of=/dev/mapper/${LOOPDEV}p3 bs=1M

echo "Creating userdata..."
sudo mkfs.ext4 /dev/mapper/${LOOPDEV}p4 -I 512 -L userdata
sync

sudo kpartx -d "/dev/${LOOPDEV}"
sudo losetup -d "/dev/${LOOPDEV}"

echo "Done, created ${IMAGES_PATH}/${IMGNAME}!"
exit 0
