obj-m := i2c_chip.o i2c_adapter.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean
