<?php

include "palette.php";

$src = imagecreatefrompng($argv[1]);
$sx  = imagesx($src);
$sy  = imagesy($src);

echo "$sx x $sy\n";

$output = '';
for ($i = 0; $i < $sy; $i += 16)
for ($j = 0; $j < $sx; $j += 16) {

    for ($y = 0; $y < 16; $y++) {

        $bits = 0;
        for ($x = 0; $x < 16; $x++) {

            $color_id = imagecolorat($src, $j+$x, $i+$y);
            $bits = ($bits << 2) | ($color_id & 3);

            // Выгрузка исходящих данных
            if (($x & 3) == 3) { $output .= chr($bits); $bits = 0; }
        }
    }
}

file_put_contents($argv[2], $output);
