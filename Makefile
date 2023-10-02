
obj-m := bme280.o 


KERNEL_DIR ?= /home/francesco/Projects/Linux_Driver_Development_with_Raspberry_Pi_Practical_Labs/kernel/linux/

all:
	make -C $(KERNEL_DIR) \
		ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- \
		M=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) \
		ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- \
		M=$(PWD) clean

deploy:
	scp *.ko root@10.0.0.10:/home/pi


