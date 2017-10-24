
# Makefile for kernel
BINCLUDE	= -I boot/include
INCLUDE		= -I include
Nostack		= -fno-stack-protector
clean :
	rm -f *.o
clean_all:
	rm -f *.o
	rm -f kernel.img 

img:	kernel.img


boot.o : boot/boot.asm
	nasm -f elf32  boot/boot.asm -o ./boot.o	


kernel.img : boot.o kernel/init.c kernel/protect.asm kernel/kernel.c kernel/video.c

	
	gcc -m32  $(INCLUDE) $(BINCLUDE) kernel/kernel.c -c -fno-stack-protector
	gcc -m32  $(INCLUDE)   kernel/init.c -c -fno-stack-protector	
	gcc -m32  $(INCLUDE)   kernel/video.c -c -fno-stack-protector
	gcc -m32  $(INCLUDE)   kernel/8259.c -c -fno-stack-protector
	gcc -m32  $(INCLUDE)   kernel/exception.c -c -fno-stack-protector

	nasm -f elf32  kernel/idtfunc.asm -o idtfunc.o
	nasm -f elf32  kernel/protect.asm -o protect.o
	nasm -f elf32 lib/memcopy.asm -o memcopy.o
	
	ld  -m elf_i386 -T linker.lds \
	boot.o  kernel.o video.o init.o memcopy.o protect.o 8259.o exception.o idtfunc.o -o kernel.img

copy:
	sudo kpartx -av a.img
	sudo mount /dev/mapper/loop0p1 /mnt/img
	sudo rm -f /mnt/img/kernel.img	
	sudo cp kernel.img /mnt/img
	ls 	/mnt/img
	sudo umount /mnt/img
	sudo losetup -d /dev/mapper/loop0p1
um:	
	ls /mnt/img
	sudo umount /mnt/img
	sudo losetup -d /dev/mapper/loop0p1


run:	
		bochs -f b

go:	img copy run


#gcc -m32 -I boot/include boot/boot.S -c
#gcc -m32 -I include -I boot/include  kernel/kernel.c -c -fno-stack-protector
#gcc -m32 -I include   kernel/init.c -c -fno-stack-protector
#gcc -m32 -I include   kernel/video.c -c -fno-stack-protector
#nasm -f elf  kernel/protect.asm -o protect.o
#ld -m elf_i386 boot.o init.o protect.o kernel.o video.o -o kernel.img

