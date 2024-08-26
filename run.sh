#!/bin/sh

gcc src/gadget.c -o gadget -march=armv8.5-a+memtag
qemu-aarch64 ./gadget
