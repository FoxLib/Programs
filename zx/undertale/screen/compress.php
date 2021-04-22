<?php

function convert($file, $fileout) {

    $width  = 200;
    $height = 104; // 107

    $src = imagecreatefrompng($file);
    $dst = imagecreatetruecolor($width, $height);
    $byte = 0;
    $out = "";

    for ($y = 0; $y < $height; $y++)
    for ($x = 0; $x < $width; $x++) {

        $color = imagecolorat($src, $x, $y);

        if ($color == 0) { // HI

            $bit = 1;
            imagesetpixel($dst, $x, $y, 0);

        } else { // LO

            $bit = 0;
            imagesetpixel($dst, $x, $y, 0xffffff);
        }

        $byte = ($byte << 1) | $bit;
        if (($x & 7) == 7) { $out .= chr($byte); $byte = 0; }
    }

    file_put_contents($fileout, $out);
    // imagepng($dst, "result.png");
}

for ($i = 1; $i < 10; $i++) {
    convert("src/screen$i.png", "screen$i.tmp");
    @unlink("screen$i.bin");
    echo `zx0 screen$i.tmp screen$i.bin`;
    @unlink("screen$i.tmp");
}
