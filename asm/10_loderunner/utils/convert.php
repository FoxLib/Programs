<?php

include "palette.php";

$src = imagecreatefrompng($argv[1]);
$dst = '';

for ($y = 0; $y < 16; $y++)
for ($x = 0; $x < 16; $x++) {

    $color = imagecolorat($src, $x, $y);

    // Разбиваю цвета на компоненты
    $rs = ($color >> 16) & 255;
    $gs = ($color >>  8) & 255;
    $bs = ($color      ) & 255;

    $color_sel = 0;
    $dist_min  = pow(2,24);

    for ($i = 0; $i < 64; $i++) {

        $rd = ($nescolors[$i] >> 16) & 255;
        $gd = ($nescolors[$i] >>  8) & 255;
        $bd = ($nescolors[$i]      ) & 255;

        $dist = pow($rs-$rd,2) + pow($gs-$gd,2) + pow($bs-$bd,2);
        if ($dist < $dist_min) {
            $dist_min  = $dist;
            $color_sel = $i;
        }
    }

    $dst .= chr($color_sel);
}

file_put_contents($argv[2], $dst);
