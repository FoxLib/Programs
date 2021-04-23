<?php

$arr = [];




// Первый экран
$arr[] = "4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4LF";
$arr[] = "4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4LF";
$arr[] = "4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 85T ";
$arr[] = "1R1 1R2 1R3 1R4 1R5 1R6 1R7 1R8 0K 1N 1R7 1R6 1R5 1R4 1R3 1R2 1R1 1R0";

// Второй экран
$arr[] = "4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4LF";
$arr[] = "4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 85T";
$arr[] = "0K 1R1 1R2 1R3 1R4 1R5 1R6 1R7 0K 1N 2R8 1R7 1R6 1R5 1R4 1R3 1R2 1R1 1R0";

// Третий экран
$arr[] = "4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4LF";
$arr[] = "4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4LF";
$arr[] = "4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 85LF";
$arr[] = "1R1 1R2 1R3 1R4 1R5 1R6 1R7 0K 2R8 1R7 1R6 1R5 1R4 1R3 1R2 1R1 1R0";

// Четвертый экран
$arr[] = "4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4LF";
$arr[] = "4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4LF";
$arr[] = "4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 140LF";
$arr[] = "1R1 1R2 1R3 1R4 1R5 1R6 1R7 0K 1N 25R8 ";

// Четвертый экран
$arr[] = "4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 40T 40T 60T";
$arr[] = "0K 1R7 1R6 1R5 1R4 1R3 1R2 1R1 4R0";

// Пятый экран
$arr[] = "4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4T 4LF";
$arr[] = "4T 4T 4T 4T 4T 4T 50T";

// ---------------------------------------------------------------------

$output = [];
foreach ($arr as $items) {

    $items = explode(' ', $items);
    foreach ($items as $item) {

        // Пропечатка буквы
        if (preg_match('~(\d+)T~', $item, $c)) {
            $output[] = $c[1];
            $output[] = 1;
        }
        // Дизеринг и перерисовка
        else if (preg_match('~(\d+)R(\d+)~', $item, $c)) {
            $output[] = $c[1];
            $output[] = 2;
            $output[] = $c[2];
        }
        // Следующий экран
        else if (preg_match('~(\d+)N~', $item, $c)) {
            $output[] = $c[1];
            $output[] = 3;
        }
        // Перенос строки
        else if (preg_match('~(\d+)LF~', $item, $c)) {
            $output[] = $c[1];
            $output[] = 4;
        }
        // Стереть буквы
        else if (preg_match('~(\d+)K~', $item, $c)) {
            $output[] = $c[1];
            $output[] = 5;
        }
        // Ожидание
        else if (preg_match('~(\d+)W~', $item, $c)) {
            $output[] = $c[1];
            $output[] = 0;
        }
    }
}

// Финализация
$output[] = 0xff; $output[] = 0xff;

// Формирование вывода
$lines = [];
$cols = [];
foreach ($output as $k => $val) {

    $cols[] = sprintf("$%02x", $val);
    if (($k & 15) == 15) {
        $lines[] = "defb " . join(",", $cols);
        $cols = [];
    }
}
if ($cols) $lines[] = "defb " . join(",", $cols);

file_put_contents("../events.asm", join("\n", $lines));

