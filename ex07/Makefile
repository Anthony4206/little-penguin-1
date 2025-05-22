obj-m := debugfs.o
debugfs-objs := main.o id.o jiffies.o foo.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

re: clean all
