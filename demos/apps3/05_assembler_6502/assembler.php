<?php

require_once "phpinc/aline.php";
require_once "phpinc/main.php";

if ($argc > 2) {
    
    $lines = loadfile($argv[1]);
    assemble($lines);
    savetofile($argv[2]);
    
} else {
    
    echo "php assembler.php <file.asm> <outfile>\n";    
}
