<?php

$files = [
    '01_brick',
    '02_ladder',
    '03_gold',
    '04_rope',
];

foreach ($files as $file) {

    echo "$file\n";
    `php convert.php ../src/$file.png ../img/$file.bin`;
}
