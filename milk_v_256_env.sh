#!/usr/bin/env bash
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
export CROSS_COMPILE=$SCRIPT_DIR/buildroot/output/host/bin/riscv64-buildroot-linux-gnu- 
export ARCH=riscv