CROSS_PREFIX := ~/pindichan/riscv64-wangzai-linux-gnu-gcc/bin/riscv64-unknown-linux-gnu-
KERNEL_DIR := ~/pindichan/linux-5.4/
arch := riscv
obj-m := rbtree.o

cc := $(CROSS_PREFIX)gcc
ld := $(CROSS_PREFIX)ld

.PHONY:all
all:
	make ARCH=$(arch)  CC=$(cc) LD=$(ld) -C $(KERNEL_DIR) M=$(shell pwd) modules V=1

.PHONY:clean
clean:
	make ARCH=$(arch) CC=$(cc) LD=$(ld) -C $(KERNEL_DIR) M=$(shell pwd) clean


