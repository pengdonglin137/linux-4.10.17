#!/bin/bash

cross_compile=arm-none-linux-gnueabi-

#make CROSS_COMPILE=$cross_compile ARCH=arm vexpress_defconfig
make CROSS_COMPILE=$cross_compile ARCH=arm menuconfig
make CROSS_COMPILE=$cross_compile ARCH=arm zImage -j4
make CROSS_COMPILE=$cross_compile ARCH=arm dtbs

