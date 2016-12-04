#!/bin/sh
SUPERIMAGE_PATH=emcraft.img
UBOOTIMAGE_PATH=u-boot.bin
KERNELIMAGE_PATH=uImage.bin

FLASH_BASE="0x08000000"
# Seek values according to MEMORY_MAP
KERNEL_OFFSET="256"

rm -f $SUPERIMAGE_PATH
dd if=/dev/zero of=$SUPERIMAGE_PATH bs=2M count=1
echo "Appending U-Boot image"
dd if=$UBOOTIMAGE_PATH of=$SUPERIMAGE_PATH
echo "Appending Kernel"
dd if=$KERNELIMAGE_PATH of=$SUPERIMAGE_PATH bs=1K seek=$KERNEL_OFFSET

