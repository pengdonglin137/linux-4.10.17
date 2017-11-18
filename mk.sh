#!/bin/bash

make ARCH=x86_64 O=./out_x86_64 x86_64_defconfig
make ARCH=x86_64 O=./out_x86_64 menuconfig
make ARCH=x86_64 O=./out_x86_64 bzImage -j8
