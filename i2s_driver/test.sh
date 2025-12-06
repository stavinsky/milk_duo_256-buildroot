modprobe snd-soc-simple-card
insmod /home/milkv/codec.ko
insmod /home/milkv/i2s_driver.ko
# aplay -D hw:0,0 -f S24_LE -c 2 -r 48000 /dev/random
# 0x04330000 - dma 
# 0x03000000 - system control 
# 0x03002000 - clock control
# devmem 0x04330188 32  #CHx_INTSTATUS
# clk 0x03002000 clk_4 0x010
# devmem 0x04330120 64
# int_mux = <0x7FC00>; /* enable bit [10..18] for CPU1(906B) */