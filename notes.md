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


###spi

modprobe -v spi-dw
modprobe -v spi-dw-mmio
duo-pinmux -r GP6 -r GP7 -r GP8 -r GP9
modprobe -v spidev



### i2s
devmem 0x03001128 32 #BCLK
devmem 0x03001130 32 #DI
devmem 0x0300112C 32 #DO
devmem 0x03001124 32 #LRCK

modprobe cv1800b-dmamux
insmod /home/milkv/i2s.ko


### to do
#### soft reset
#### set up mux 
target is i2s2

base address 0x04108000

devmem 0x03001128 32  7 #BCLK
devmem 0x03001130 32  7 #DI
devmem 0x0300112C 32  7 #DO
devmem 0x03001124 32  7 #LRCK



# 1) Use APB interface for EPHY
devmem 0x03009804 32 0x0001      # rg_ephy_apb_rw_sel = 1

# 2) Short PLL stable time (doesn't matter much for us, but keep as in ref code)
devmem 0x03009808 32 0x0001      # rg_ephy_pll_stable_cnt = 1

# 3) Release EPHY digital reset (keep rest at default)
devmem 0x03009800 32 0x0905      # rg_ephy_dig_rst_n = 1

# small delay to let PLL lock
sleep 0.001

# 4) Select page 5 in EPHY analog reg page
devmem 0x0300907C 32 0x0500      # page_sel_mode0 = 5

# 5) Route signals to GPIO from “top”
devmem 0x03009078 32 0x0F00

# 6) Enable input/output on RXP/RXM pairs
devmem 0x03009074 32 0x0606
devmem 0x03009070 32 0x0606


https://community.milkv.io/t/duo256m-add-max98357a-sound-card-done/1494
https://developer.sophgo.com/thread/472.html


### clock


devmem 0x03002004 32 0xf400e1b2



0x03002118 div_clk_audsrc 


apll_frac_div_n
apll_frac_div_m


Adjust fractionalPLL process:

1. WhenusingthisPLL, theIP clockmust be turned off orXTAL orother stableclocksources mustbe selected.
2. Configure *_ssc_syn_src_en toenablethe synthesizer clock
3. Configure *_ssc_syn_setaccordingtoPLLfrequency requirements,
4. Toggle *_ssc_syn_up to makethe configuration takeeffect
5. Configure the*_pll_csrregisteraccordingtothe integer PLL parametertable
5. Configure the*_pll_csrregisteraccordingtothe integer PLL parametertable
6. Clear *_pll_pwd


pll base 0x03002800
apll_ssc_syn_ctrl 0x050 
apll_ssc_syn_set 0x054 
apll_frac_div_ctrl 90
apll_frac_div_m 94 
apll_frac_div_n 98


0x03002000

div_clk_sdma_aud2 0x0a0 
devmem 0x030020a0 32 0x800009 hex( 1 | 128  << 16 | 1 <<3 )


devmem 0x030020a0 32 0x100009


aplay -D hw:0,0 -f S24_LE -c 2 -r 48000 /dev/zero
// dma remap
devmem 0x03000154 32
devmem 0x03000158 32


devmem 0x04120028 32 0x01070007 # somthing with threshold did the job and dma is not failing anymore


### listen to the raw file on the host
 gst-launch-1.0 filesrc location=file.raw ! audio/x-raw,format=S24_32LE,rate=48000,channels=2,layout=interleaved ! audioconvert ! audio/x-raw,format=F32LE  ! audioresample ! autoaudiosink
gst-launch-1.0 filesrc location=file.raw ! audio/x-raw,format=S24_32LE,rate=48000,channels=2,layout=interleaved ! audioconvert ! audioresample ! autoaudiosink