#!/bin/bash

#make ARCH=arm CROSS_COMPILE=arm-linux- tq2440_dt_defconfig

make ARCH=arm CROSS_COMPILE=arm-linux- uImage -j4
make ARCH=arm CROSS_COMPILE=arm-linux- dtbs

dtc -I dtb -O dts -o tq2440.dts arch/arm/boot/dts/s3c2440-tq2440-dt.dtb
