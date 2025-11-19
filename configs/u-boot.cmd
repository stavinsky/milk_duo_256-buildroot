setenv kernel_addr_r 0x80200000
setenv fdt_addr_r    0x82000000
setenv bootargs "root=/dev/mmcblk0p2 rw rootwait console=ttyS0,115200"

fatload mmc 0:1 ${kernel_addr_r} Image
fatload mmc 0:1 ${fdt_addr_r} 1.dtb

booti ${kernel_addr_r} - ${fdt_addr_r}
