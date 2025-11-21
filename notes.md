cp arch/riscv/boot/dts/sophgo/sg2002-licheerv-nano-b.dtb /media/stavinskii/boot/1.dtb
cp arch/riscv/boot/Image* /media/stavinskii/boot/

setenv kernel_addr_r 0x80200000
setenv fdt_addr_r    0x82000000
setenv bootargs "root=/dev/mmcblk0p2 rw rootwait console=ttyS0,115200"

fatload mmc 0:1 ${kernel_addr_r} Image
fatload mmc 0:1 ${fdt_addr_r} 1.dtb

booti ${kernel_addr_r} - ${fdt_addr_r}



fatload mmc 0:1 0x80200000 Image



setenv bootcmd 'mmc dev 0; fatload mmc 0:1 0x80100000 boot.scr; source 0x80100000'



```
usb network 

#!/bin/sh

modprobe dwc2
modprobe configfs
modprobe libcomposite
mount -t configfs none /sys/kernel/config

G=/sys/kernel/config/usb_gadget/g1

mkdir -p $G
echo 0x1d6b > $G/idVendor    # Linux Foundation
echo 0x0104 > $G/idProduct   # Multifunction Composite Gadget

mkdir -p $G/strings/0x409
echo "1234567890" > $G/strings/0x409/serialnumber
echo "MilkV" > $G/strings/0x409/manufacturer
echo "Duo USB Network" > $G/strings/0x409/product

mkdir -p $G/configs/c.1/strings/0x409
echo "ECM network" > $G/configs/c.1/strings/0x409/configuration

# Create ECM interface
mkdir -p $G/functions/ecm.usb0
echo "02:12:34:56:78:9a" > $G/functions/ecm.usb0/host_addr
echo "02:aa:bb:cc:dd:ee" > $G/functions/ecm.usb0/dev_addr

# Link function to configuration
ln -s $G/functions/ecm.usb0 $G/configs/c.1/

# Enable gadget
UDC=$(ls /sys/class/udc | head -n 1)
echo $UDC > $G/UDC

# Assign IP
ifconfig usb0 10.0.0.1 up
```