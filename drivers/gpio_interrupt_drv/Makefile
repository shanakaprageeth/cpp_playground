# setup driver env variables
DRIVER_NAME = gpio_interrupt_drv
obj-m := $(DRIVER_NAME).o

# all build target builds the modules
all: module

# custom build command to build module target to build kernel module
module:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

# clean kernel module build outputs
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clear

# build target all and verify the kernel module is loaded to the system or load it
check: all
	lsmod | grep $(DRIVER_NAME) || $(MAKE) insmod

# target to insert kernel module to system
insmod:
	sudo insmod $(DRIVER_NAME).ko


# target to remove kernel module from system
rmmod:
	sudo rmmod $(DRIVER_NAME) || true

# target to create dummy device on userland for static device test
#add_device:
#	sudo mknod /dev/gpio_dev c 99 0


#target write/read the device
rw_device:
	sleep 2
	sudo chmod 666 /dev/gpio_dev
	echo "255" > /dev/gpio_dev
	head -n 1 /dev/gpio_dev

# target test device
test: all rmmod insmod rw_device rmmod

