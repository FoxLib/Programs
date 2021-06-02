all:
	fasm main.asm
	z80asm z80.asm -o z80.bin
	dd conv=notrunc if=z80.bin of=disk.img bs=512 seek=2 2>/dev/null
	rm -f *.lock
	bochs -f c.bxrc -q > /dev/null 2>&1

disk:
	dd if=/dev/zero of=disk.img bs=1024 count=65536
