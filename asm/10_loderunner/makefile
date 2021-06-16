all:
	fasm main.asm
	#copy main.com d:\dosbox > nul
	cp main.com ~/dosbox/
	dosbox
convert:
	php utils/convert.php src/01_brick.png img/01_brick.bin
	php utils/convert.php src/02_ladder.png img/02_ladder.bin
	php utils/convert.php src/03_gold.png img/03_gold.bin
	php utils/convert.php src/04_rope.png img/04_rope.bin
