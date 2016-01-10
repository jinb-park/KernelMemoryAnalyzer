#!/bin/sh

./build.sh
cp -f Build/* /home/pjb1027/OpenSource/busybox-1.24.1/_install/
mkdir -p /home/pjb1027/OpenSource/busybox-1.24.1/_install/usr/lib/
cp -f Build/*.so /home/pjb1027/OpenSource/busybox-1.24.1/_install/usr/lib/

cd /home/pjb1027/OpenSource/busybox-1.24.1/
./make_rootfs.sh

cd ~/bin/
./qemu_arm32.sh
