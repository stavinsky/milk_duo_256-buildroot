#!/bin/sh
set -e
TARGET_DIR="$1"

cd $TOPDIR
mkdir -p tmp
cd tmp
if [ ! -d "./fiptool" ]; then
  wget https://github.com/sophgo/fiptool/archive/refs/heads/master.zip -O fiptool.zip
  unzip fiptool.zip
  mv fiptool-master fiptool
fi

cd fiptool
python3 ./fiptool \
    --fsbl data/fsbl/cv181x.bin  \
    --ddr_param data/ddr_param.bin \
    --opensbi $BINARIES_DIR/fw_dynamic.bin \
    --uboot $BINARIES_DIR/u-boot.bin \
    --rtos ./data/cvirtos.bin
cp fip.bin $BINARIES_DIR