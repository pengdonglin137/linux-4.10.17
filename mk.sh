#!/bin/bash
#make ARCH=i386 i386_defconfig
make ARCH=i386 menuconfig
make ARCH=i386 bzImage -j8

#make ARCH=x86_64 O=./out_x86_64 x86_64_defconfig
#make ARCH=x86_64 O=./out_x86_64 menuconfig
#make ARCH=x86_64 O=./out_x86_64 bzImage -j8
