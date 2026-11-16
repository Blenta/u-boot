#
# Copyright (C) 2011 Samsung Electronics Co., Ltd.
#              http://www.samsung.com/
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
####################################

if [ -z $1 ]
then
    echo "usage: ./sd_fusing.sh <SD Reader's device file>"
    exit 0
fi

if [ -b $1 ]
then
    echo "$1 reader is identified."
else
    echo "$1 is NOT identified."
    exit 0
fi

####################################
#<verify device>

BDEV_NAME=`basename $1`
BDEV_SIZE=`cat /sys/block/${BDEV_NAME}/size`

if [ ${BDEV_SIZE} -le 0 ]; then
	echo "Error: NO media found in card reader."
	exit 1
fi

if [ ${BDEV_SIZE} -gt 32000000 ]; then
	echo "Error: Block device size (${BDEV_SIZE}) is too large"
	exit 1
fi

####################################
# check files

E4412_UBOOT=../../u-boot.bin
E4412_UBOOT_SPL=../../spl/tiny4412-spl.bin
KERNEL=zImage-3.0.86
RAMDISK=ramdisk.img

if [ ! -f ${E4412_UBOOT} ]; then
	echo "Error: u-boot.bin NOT found, please build it & try again."
	exit -1
fi

if [ ! -f ${E4412_UBOOT_SPL} ]; then
	echo "Error: tiny4412-spl.bin NOT found, please build it & try again."
	exit -1
fi

####################################
# fusing images

block=512
reserve_size=512
bl1_size=$((8*1024)) #8KB
bl2_size=$((16*1024))	#16KB
uboot_size=$((1024*1024))	#1024KB
env_size=$((16*1024))	#16KB
kernel_size=$((10*1024*1024))	#10MB
ramdisk_size=$((10*1024*1024))	#10MB

echo "-----------------size----------------------"
echo block = $block
echo reserve_size = $reserve_size
echo bl1_size = $bl1_size
echo bl2_size = $bl2_size
echo uboot_size = $uboot_size
echo env_size = $env_size
echo kernel_size = $kernel_size
echo ramdisk_size = $ramdisk_size

signed_bl1_position=$((reserve_size/block))
bl2_position=$((signed_bl1_position+bl1_size/block))
uboot_position=$((bl2_position+bl2_size/block))
env_position=$((uboot_position+uboot_size/block))
kernel_position=$((env_position+env_size/block))
ramdisk_position=$((kernel_position+kernel_size/block))

echo "-----------------offset----------------------"
echo signed_bl1_position = $signed_bl1_position
echo bl2_position = $bl2_position
echo uboot_position = $uboot_position
echo env_position = $env_position
echo kernel_position = $kernel_position
echo ramdisk_position = $ramdisk_position

#<BL1 fusing>
echo "---------------------------------------"
echo "BL1 fusing"
dd iflag=dsync oflag=dsync if=./E4412_N.bl1.bin of=$1 seek=$signed_bl1_position

#<BL2 fusing>
echo "---------------------------------------"
echo "BL2 fusing"
dd iflag=dsync oflag=dsync if=${E4412_UBOOT_SPL} of=$1 seek=$bl2_position

#<u-boot fusing>
echo "---------------------------------------"
echo "u-boot fusing"
dd iflag=dsync oflag=dsync if=${E4412_UBOOT} of=$1 seek=$uboot_position

#<kernel fusing>
echo "---------------------------------------"
echo "kernel fusing"
dd iflag=dsync oflag=dsync if=${KERNEL} of=$1 seek=$kernel_position

#<ramdisk fusing>
echo "---------------------------------------"
echo "ramdisk fusing"
dd iflag=dsync oflag=dsync if=${RAMDISK} of=$1 seek=$ramdisk_position

#<flush to disk>
sync

####################################
#<Message Display>
echo "---------------------------------------"
echo "U-boot image is fused successfully."
echo "Eject SD card and insert it again."

