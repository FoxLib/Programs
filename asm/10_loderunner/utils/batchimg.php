<?php

$files = [
    '01_brick',
    '02_ladder',
    '02_ladder1',
    '02_ladder2',
    '03_gold',
    '03_gold1',
    '03_gold2',
    '04_rope',
    '05_iron',
];

foreach ($files as $file) {

    echo "$file\n";
    echo `php convert.php ../src/$file.png ../img/$file.bin`;
}
