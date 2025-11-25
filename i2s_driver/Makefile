obj-m += i2s_driver.o
obj-m += codec.o
obj-m += debug_irq.o

# Adjust KDIR to point to your built kernel tree (not the source root, the build dir)
KDIR ?= /path/to/linux/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
