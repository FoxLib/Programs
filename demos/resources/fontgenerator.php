<?php

$font = file_get_contents("rusfont8x8.fnt");
for ($i = 0; $i < 256; $i++) {

    $item = [];
    for ($j = 0; $j < 8; $j++) {
        $ch = ord($font[$i*8 + $j]);
        $ch = str_pad(dechex($ch), 2, '0', STR_PAD_LEFT);
        $item[] = "0x$ch";
    }
    echo "{".join(", ", $item) . "},\n";
}
