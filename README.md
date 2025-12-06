### About
I am really in love with buildroot. But everything is new for me yet. 
So I decided to improve my skills by creating very basic image for milk duo 256M,
The main goal is to make bare minimal bootable image with minimal customization from mainline repos of kernel buildroot and uboot. Everything should be simple and relies on buildroot documentation. 


What you can expect: 
 - mainline kernel (unfortunately release candidate) with patches from sophgo 
 - mainline u-boot (also rc, on 2025.10 fatload fails everytime)
 - it builds, boots. 
 - usb peripheral works. Tested with ethernet card emulation
   - network card starts with ip 10.42.0.1
   - ssh milkv@10.42.0.1 pass: `1`. You can change password in configs/users.txt(keep `=` prefix) 
 - middleware are not compiled yet
 - don't know how to dial with rtos
 - GPIO 0-4 works fine. But we need to setup pinmux by duo-pinmux tool. Added by custom buildroot package
 - rootfs has minimal packages. if you need any package install from `make menuconfig` from buildroot folder

comments issues and PR's are very welcome!

### structure
 - configs - all the configs
 - configs/dt - dts files from the kernel 6.18rc6 with patches
 - patches - 3 patches mostly for device tree 
 - Makefile - very simple self documenting Makefile
 - notes.md - some draft notes and commands for copy and paste. can be ignored
 - dl - to avoid repeating downloads, this folder moved up from buildroot. this is for downloads for buildroot
 - configs/u-boot_defconfig - milkv_duo_defconfig from u-boot wih minor changes. 

### requirements 
 - https://buildroot.org/downloads/manual/manual.html#requirement
 - mtools is also required to build `sdcard.img`

### the process on linux
1. `make prepare` will download buildroot and prepare toolchain. can take half an hour or more
2. `cd buildroot` now you have buildroot dir that can be used as usual. On this step and later everything you do, you need to do from this folder
    - `make` will build the project
    - output/images - folder with binaries. 
    - output/images/sdcard.img is your image. 
3. `make uboot` 
4. `make`
5. `sudo dd if=output/images/sdcard.img  of=/dev/sdX conv=fsync status=progress` will make sd card for you.



### Usefull Buildroot commands
 - `make BR2_DEFCONFIG=../configs/buildroot-config savedefconfig` - update buildroot configuration 
 - `make uboot-dirclean opensbi-dirclean linux-dirclean` - reset builds for main components so `make` will build them from scratch


 ### make targets 
  - make buildroot - download buildroot
  - make toolchain - run `make toolchain` form buildroot folder
  - `make all` should make the target. results in `buildroot/output/images` 
