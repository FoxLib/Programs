all:
	fasm main.asm
	#copy main.com d:\dosbox > nul
	cp main.com ~/dosbox/
	dosbox
convert:
	php utils/pngto2bin.php src/sprites.png img/sprites.bin
	php utils/pngto2bin.php src/tilemap.png img/tilemap.bin
