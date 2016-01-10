#!/bin/sh

# 1. LKM Build

# ARM
#ARCH="arm"
#CROSS_COMPILE="arm-linux-gnueabi-"
#KDIR="/home/pjb1027/OpenSource/linux"

# Intel
ARCH=""
CROSS_COMPILE=""
KDIR="/lib/modules/3.16.0-45-generic/build"

mkdir Build

cd LKM
cp -f ../Common/Command.h .
cp -f ../Common/Structure.h .
cp -f ../Common/KmaAPI.h .

make KDIR="$KDIR"
#make CROSS_COMPILE="$CROSS_COMPILE" ARCH="$ARCH" KDIR="$KDIR"
cp -f *.ko ../Build
cd ../

# 2. APP Build

cd APP
cp -f ../Common/Command.h .
cp -f ../Common/Structure.h .
cp -f ../Common/KmaAPI.h .

#CROSS_COMPILE="arm-linux-gnueabi"
make CROSS_COMPILE="$CROSS_COMPILE"
cp -f KernelMemoryAnalyzer ../Build
cp -f *.so ../Build
cd ../

# 3. Clean

./clean.sh

