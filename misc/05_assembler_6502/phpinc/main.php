<?php

/*
 * Главный модуль
 */

class RT {

    // Список Define
    static $defines = [];

    // Откуда начинается программа
    static $org = 0x8000;

    // Список меток
    static $labels = [];

    // Список точек для переходов (1 или 2 байтные)
    static $branches = [];

    // Программа
    static $program = [];
};

// Загрузка файла в память
function loadfile($file) {

    if (file_exists($file)) {

        $lines = array_map('trim', file($file));
        foreach ($lines as $i => $v) {        

            $t = preg_replace("~;.+$~", "", $v); // -- доработать, чтобы удалялись только комментарии а не ";" или ';'
            $t = trim($t);
            
            // Включение других файлов
            if (preg_match('~^include (.+)$~', $t, $cp)) {
                
                $news = loadfile($cp[1]);
                $lines = array_merge($lines, $news);
            }

            if ($t) {
                $lines[$i] = $t;
            } else {
                unset($lines[$i]);
            }

        }
        return array_values($lines);

    } else {

        die("Файл не найден: $file\n");
    }

}

function glue($w, $n) {
    return join(" ", array_slice($w, $n));
}

// Ассемблирование полученных строк
function assemble($lines) {

    foreach ($lines as $id => $line) {

        // @todo -- замена define -->

        $word = preg_split("~\s+~", $line);
        $first = strtolower($word[0]);

        switch ($first) {

            // Определить переменную
            case 'define':

                RT::$defines[ $word[1] ] = glue($word, 2);
                break;

            // Определить начало позиции
            case 'org':

                RT::$org = prepare_numeric( glue($word, 1) );
                break;
                
            case 'db':
            
                parse_data( glue($word, 1) );
                break;

            // Ассемблировать линию
            default:

                assemble_line( $word );
                break;
        }
    }

    foreach (RT::$branches as $addr => $item) {

        $addr++;

        // Поиск адрес метки
        $label_name = $item[1];
        if (!isset(RT::$labels[ $label_name ])) {
            die("Label '$label_name' not found\n");
        }
        $label_addr = RT::$labels[ $label_name ];

        // Ближний переход
        if ($item[0] == 'NEAR') {

            $rel = $label_addr - ($addr + 1);
            if ($rel < -128 || $rel > 127) {
                die("Branch $label_name out of range\n");
            } else {
                RT::$program[ $addr ] = $rel & 0xff;
            }

        }
        // Дальний переход
        else if ($item[0] == 'FAR') {

            RT::$program[ $addr + 0 ] = $label_addr & 0xff;
            RT::$program[ $addr + 1 ] = ($label_addr >> 8) & 0xff;
        }
        // HIGH адрес
        else if ($item[0] == 'HI') {
            RT::$program[ $addr ] = ($label_addr >> 8) & 0xff;
        }
        // LO адрес
        else if ($item[0] == 'LO') {
            RT::$program[ $addr ] = $label_addr & 0xff;
        }
    }    
}

function savetofile($file) {
    
    $out = '';
    for ($i = 0x8000; $i <= 0xFFFF; $i++) {
        
        $out .= chr(isset(RT::$program[$i]) ? RT::$program[$i] : 0x00);
    }
    
    file_put_contents($file, $out);
}
