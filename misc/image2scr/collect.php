<?php

$list = glob("src/u_*.jpg");
sort($list);
foreach ($list as $file) {

    echo $file."\n";
    $base = str_replace('.jpg', '.png', basename($file));
    $bash = "convert $file -auto-gamma -resize 256x192^ -gravity Center -extent 256x192 input.ppm";
    `$bash`;
    `./main bw3 input.ppm output.ppm output.scr`;
    #`./main org input.ppm output.ppm output.scr`;
    `convert output.ppm dst/$base`;
}
