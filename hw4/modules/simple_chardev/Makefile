ifneq ($(KERNELRELEASE),)
M ?= $(shell basename $(PWD))
obj-m := $(M).o

else
ARCH            ?= aarch64
KERNEL_ARCH     ?= arm64
CROSS_COMPILE   ?= ${ARCH}-linux-gnu-
CC              ?= ${CROSS_COMPILE}gcc
PWD             ?= $(shell pwd)
KDIR             ?= $(PWD)/../../linux-4.11/

default:
	ARCH=$(KERNEL_ARCH) CROSS_COMPILE=$(CROSS_COMPILE) $(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
endif

test:
	$(CROSS_COMPILE)gcc -static -x c simple_chardev.c.test -o simple_chardev.ko.test

clean:
	rm -rf *.ko *.o *.mod.c .*.cmd .tmp_versions *.symvers *.order *.ko.test
