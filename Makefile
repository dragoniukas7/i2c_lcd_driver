obj-m := i2clcd.o
i2clcd-objs := i2clcd_main.o char_device.o lcd_functions.o
KDIR := /home/studentas/Documents/linux-5.16.12
PWD := $(shell pwd)
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
