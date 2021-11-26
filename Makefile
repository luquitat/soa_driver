obj-m := usb_driver.o

KDIR = /lib/modules/$(shell uname -r)/build

modules:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean
