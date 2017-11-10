#!/bin/bash

cross_compile=aarch64-linux-gnu-

#make CROSS_COMPILE=$cross_compile ARCH=arm64 defconfig
make CROSS_COMPILE=$cross_compile ARCH=arm64 menuconfig
make CROSS_COMPILE=$cross_compile ARCH=arm64 Image -j4
