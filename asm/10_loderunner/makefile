all:
	fasm main.asm
	#copy main.com d:\dosbox > nul
	cp main.com ~/dosbox/
	dosbox
convert:
	cd utils && php batchimg.php
