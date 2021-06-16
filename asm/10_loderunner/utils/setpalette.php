<?php

$colors = [];

function setPalette($id, $r, $g, $b) {

    global $colors;
    $colors[$id] = [$r*65536 + $g*256 + $b, $r, $g, $b];
}

setPalette(0x00, 117, 117, 117);
setPalette(0x01, 39,  27,  143);
setPalette(0x02, 0,   0,   171);
setPalette(0x03, 71,  0,   159);
setPalette(0x04, 143, 0,   119);
setPalette(0x05, 171, 0,   19);
setPalette(0x06, 167, 0,   0);
setPalette(0x07, 127, 11,  0);
setPalette(0x08, 67,  47,  0);
setPalette(0x09, 0,   71,  0);
setPalette(0x0A, 0,   81,  0);
setPalette(0x0B, 0,   63,  23);
setPalette(0x0C, 27,  63,  95);
setPalette(0x0D, 0,   0,   0);
setPalette(0x0E, 0,   0,   0);
setPalette(0x0F, 0,   0,   0);

setPalette(0x10, 188, 188, 188);
setPalette(0x11, 0,   115, 239);
setPalette(0x12, 35,  59,  239);
setPalette(0x13, 131, 0,   243);
setPalette(0x14, 191, 0,   191);
setPalette(0x15, 231, 0,   91);
setPalette(0x16, 219, 43,  0);
setPalette(0x17, 203, 79,  15);
setPalette(0x18, 139, 115, 0);
setPalette(0x19, 0,   151, 0);
setPalette(0x1A, 0,   171, 0);
setPalette(0x1B, 0,   147, 59);
setPalette(0x1C, 0,   131, 139);
setPalette(0x1D, 0,   0,   0);
setPalette(0x1E, 0,   0,   0);
setPalette(0x1F, 0,   0,   0);

setPalette(0x20, 255, 255, 255);
setPalette(0x21, 63,  191, 255);
setPalette(0x22, 95,  151, 255);
setPalette(0x23, 167, 139, 253);
setPalette(0x24, 247, 123, 255);
setPalette(0x25, 255, 119, 183);
setPalette(0x26, 255, 119, 99);
setPalette(0x27, 255, 155, 59);
setPalette(0x28, 243, 191, 63);
setPalette(0x29, 131, 211, 19);
setPalette(0x2A, 79,  223, 75);
setPalette(0x2B, 88,  248, 152);
setPalette(0x2C, 0,   235, 219);
setPalette(0x2D, 0,   0,   0);
setPalette(0x2E, 0,   0,   0);
setPalette(0x2F, 0,   0,   0);

setPalette(0x30, 255, 255, 255);
setPalette(0x31, 171, 231, 255);
setPalette(0x32, 199, 215, 255);
setPalette(0x33, 215, 203, 255);
setPalette(0x34, 255, 199, 255);
setPalette(0x35, 255, 199, 219);
setPalette(0x36, 255, 191, 179);
setPalette(0x37, 255, 219, 171);
setPalette(0x38, 255, 231, 163);
setPalette(0x39, 227, 255, 163);
setPalette(0x3A, 171, 243, 191);
setPalette(0x3B, 179, 255, 207);
setPalette(0x3C, 159, 255, 243);
setPalette(0x3D, 0,   0,   0);
setPalette(0x3E, 0,   0,   0);
setPalette(0x3F, 0,   0,   0);

ksort($colors);
for ($i = 0; $i < 64; $i++) {

    $color = $colors[$i];
    echo sprintf("0x%06x, ", $color[0]);
}
