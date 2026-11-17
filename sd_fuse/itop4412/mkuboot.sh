#!/bin/bash

cat E4412_N.bl1.bin ../../spl/itop4412-spl.bin ../../u-boot.bin > u-boot-itop4412.bin

if [ -f u-boot-itop4412.bin ] ; then
	echo "created u-boot-itop4412.bin success!!!"
else
	echo "created u-boot-itop4412.bin failed!!!"
	exit 0
fi