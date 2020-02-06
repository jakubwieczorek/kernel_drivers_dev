#!/bin/bash
BRPATH=`pwd`/buildroot-2019.08.1
qemu-system-arm -M vexpress-a9 -m 512 -nographic -dtb $BRPATH/output/images/vexpress-v2p-ca9.dtb -kernel $BRPATH/output/images/zImage -append console=ttyAMA0,115200
