all:
	fasm main.asm
	dd conv=notrunc if=main.bin of=disk.img bs=510 count=1
	dd conv=notrunc if=bindizzy.bin of=disk.img bs=512 seek=1 count=64
	rm -f *.lock
	bochs -f c.bxrc -q > /dev/null 2>&1
