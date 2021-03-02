
if (gcc -Os -o i386 i386.c -lglut -lGL)
then

    ./i386

fi

# objdump -M intel-mnemonic -S i386
