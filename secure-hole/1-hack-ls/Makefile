
obj-m := hackls.o

KDIR := /usr/src/linux-headers-3.2.0-29-generic-pae/

all:
	make -C $(KDIR)	SUBDIRS=$(PWD) 	modules

clean:
	rm -rf *.o *.ko *.mod.* *.cmd 
	rm -rf .*
	rm modules.order Module.symvers
