.PHONY: clean toolchain all
BR2_DL_DIR=$(shell realpath ./dl)
export BR2_DL_DIR
clean: 
	rm -rf out/fw_dynamic.bin
	rm -rf out/fip.bin

buildroot: 
	git clone --depth 1 --branch master https://github.com/buildroot/buildroot.git
toolchain: buildroot
	mkdir -p $(BR2_DL_DIR) 
	$(MAKE) -C buildroot/ BR2_DEFCONFIG=../configs/buildroot-config BR2_EXTERNAL=../br-external defconfig
	$(MAKE) -C buildroot/ toolchain 

all: toolchain
	$(MAKE) -C buildroot/ uboot
	$(MAKE) -C buildroot/ 	

