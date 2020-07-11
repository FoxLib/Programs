<?php

$fc = file_get_contents("landscape/256_00.bin");

$im = imagecreatetruecolor(256, 256);
for ($i = 0; $i < 256; $i++) {
    for ($j = 0; $j < 256; $j++) {
        
        $c = ord($fc[ $j + $i*256 ]);
        imagesetpixel($im, $j, $i, $c + $c*256 + $c*65536);
    }
    
}

imagepng($im, "preview.png");
