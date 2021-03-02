# sudo mount disk.img -t vfat -o loop,rw,uid="fox",sync,offset=$[1048576] disk/
if (fasm kernel.asm)
then

    mv kernel.bin disk/kernel.bin
    bochs -f c.bxrc -q

fi
