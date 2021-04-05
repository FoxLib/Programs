#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dither.cc"

/**
 * Конверсия PPM-файла 256x192 в SCR
 */

int main(int argc, char** argv) {

    DITHER m;

    if (argc <= 1) { printf("Требуется имя файла\n"); return 1; }
    if (argc < 3) { printf("Требуется аргумент\n"); return 1; }

    int mode;
    if      (strcmp(argv[1], "org") == 0) mode = 1;
    else if (strcmp(argv[1], "bw") == 0) mode = 2;
    else if (strcmp(argv[1], "floyd") == 0) mode = 3;
    else { printf("Неверный режим"); return 1; }

    if (m.load_ppm(argv[2])) { printf("Должен быть файл 256x192\n"); return 1; }

    // Нормализация яркости
    m.halftoned();
    m.copy_source();

    // Обычный режим работы
    if (mode == 1) {

        m.dither_nearest(1);
        m.clashing();
        m.scr2source();
    }
    // Черно-белый
    else if (mode == 2) {

        m.grayscale();
        m.dither_floyd(1);
        m.clashing();
    }
    // Цветной флойда
    else if (mode == 3) {

        m.dither_floyd(1);
        m.clashing();
        m.scr2source();
    }

    // Вывод данных
    m.save_ppm(argc > 3 ? argv[3] : "result_output.ppm");
    m.save_scr(argc > 4 ? argv[4] : "result_output.scr");

    return 0;
}
