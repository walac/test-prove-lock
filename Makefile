KERNEL_VERSION := $(shell uname -r)
MAKE_CMD := make -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD)
obj-m += provelock.o

all: module

distclean: clean
	rm -f *.ko

module:
	$(MAKE_CMD) modules

clean:
	$(MAKE_CMD) clean
 
.PHONY: distclean clean clean
