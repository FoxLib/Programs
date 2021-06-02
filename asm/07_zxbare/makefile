all:
	fasm main.asm
	z80asm z80.asm -o z80.bin
	# Главная программа
	dd conv=notrunc if=main.bin of=disk.img bs=512 seek=1 2>/dev/null
	# ROM
	dd conv=notrunc if=basic.rom of=disk.img bs=512 seek=129 2>/dev/null
	# Проверочная программа
	dd conv=notrunc if=z80.bin of=disk.img bs=512 seek=193 2>/dev/null
	make bochs

bochs:
	rm -f *.lock
	bochs -f c.bxrc -q > /dev/null 2>&1

disk:
	dd if=/dev/zero of=disk.img bs=1024 count=65536
