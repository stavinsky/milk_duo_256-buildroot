.PHONY: clean u-boot-%

UBOOT_FILES := out/u-boot.bin out/u-boot.dtb

u-boot: 
	git clone https://github.com/u-boot/u-boot.git --depth=1

opensbi:
	git clone https://github.com/riscv-software-src/opensbi.git --depth=1

fiptool: 
	git clone https://github.com/sophgo/fiptool.git --depth=1

kernel: 
	git clone https://github.com/torvalds/linux.git --depth=1 --branch=v6.17 kernel

u-boot/.config: u-boot 
# 	$(MAKE) -C u-boot/ milkv_duo_defconfig 
	cp configs/u-boot_defconfig u-boot/.config
	$(MAKE) -C u-boot/ olddefconfig 
 
# u-boot: $(UBOOT_FILES) 

out/u-boot.bin:  u-boot/.config
	$(MAKE) -C u-boot/
	cp -a u-boot/u-boot.bin out/
out/u-boot.dtb: out/u-boot.bin
	cp -a u-boot/u-boot.dtb out/

clean: 
	rm -rf $(UBOOT_FILES)
	rm -rf out/fw_dynamic.bin
	rm -rf out/fip.bin
	-$(MAKE) -C u-boot/ clean
	-$(MAKE) -C opensbi/ clean

u-boot-%:
	$(MAKE) -C u-boot/ $*

kernel-%:
	$(MAKE) -C kernel/ $*

buildroot-%:
	$(MAKE) -C buildroot/ $*

u-boot-menuconfig:
	$(MAKE) -C u-boot menuconfig
	$(MAKE) -C u-boot/ savedefconfig
	cp u-boot/defconfig configs/u-boot_defconfig

out/fw_dynamic.bin: opensbi out/u-boot.dtb 
	$(MAKE) -C opensbi/ PLATFORM=generic FW_FDT_PATH=../out/u-boot.dtb
	cp opensbi/build/platform/generic/firmware/fw_dynamic.bin out/fw_dynamic.bin


out/fip.bin: fiptool out/fw_dynamic.bin out/u-boot.bin
	cd fiptool &&\
	python3 fiptool \
		--fsbl data/fsbl/cv181x.bin     \
		--ddr_param data/ddr_param.bin     \
		--opensbi ../out/fw_dynamic.bin     \
		--uboot ../out/u-boot.bin     \
		--rtos data/cvirtos.bin
	cp fiptool/fip.bin out/fip.bin

out/boot.scr: configs/u-boot.cmd
	mkimage -A arm -T script -C none -n "ITB Boot Script" -d configs/u-boot.cmd out/boot.scr

out/1.dtb: 
	cp configs/duo256.dts kernel/arch/riscv/boot/dts/sophgo/
	$(MAKE) -C kernel dtbs 
	cp kernel/arch/riscv/boot/dts/sophgo/duo256.dtb out/1.dtb 

