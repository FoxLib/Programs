all:
	fasm main.asm
	z80asm hello.asm -o hello.bin
	dd conv=notrunc if=main.bin of=disk.img bs=510 count=1 2>/dev/null
	dd conv=notrunc if=bindizzy.bin of=disk.img bs=512 seek=1 count=64 2>/dev/null
	dd conv=notrunc if=hello.bin of=disk.img bs=512 seek=47 count=1 2>/dev/null
	rm -f *.lock
	bochs -f c.bxrc -q > /dev/null 2>&1
