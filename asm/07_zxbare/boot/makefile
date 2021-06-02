all:
	fasm boot.asm
	dd conv=notrunc if=boot.bin of=../disk.img bs=512 count=1 2>/dev/null
	cd .. && make bochs
