#!/usr/bin/env bash
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
export KDIR=$SCRIPT_DIR/buildroot/output/build/linux-6.18/
# export KDIR=$SCRIPT_DIR/buildroot/output/build/linux-for-next/
export CROSS_COMPILE=$SCRIPT_DIR/buildroot/output/host/bin/riscv64-buildroot-linux-gnu- 
export ARCH=riscv
