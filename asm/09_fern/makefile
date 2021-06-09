all:
	php valuegen.php > params.asm
	fasm main.asm
	mv main.com ~/dosbox/
	dosbox
