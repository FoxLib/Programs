#include <stdio.h>
#include <stdlib.h>
#include "dither.cc"

/**
 * Конверсия PPM-файла 256x192 в SCR
 */

int main(int argc, char** argv) {

    DITHER m;

    if (argc <= 1) { printf("Требуется имя файла\n"); return 1; }
    if (m.load_ppm(argv[1])) { printf("Должен быть файл 256x192\n"); return 1; }

    // Нормализация яркости
    m.halftoned();

/*
    m.copy_source();
    m.grayscale();

    // Метод дизеринга
    // --------------------
    //m.dither_nearest(0);
    //m.dither_floyd(0);
    m.dither_bayer(0);          // В dest[0] находится черно-белое изображение
    // --------------------
*/
    // Метод дизеринга
    // --------------------
    m.copy_source();
    //m.dither_bayer(1);          // В dest[1] находится цветное
    m.dither_floyd(1);          // В dest[1] находится цветное
    //m.dither_nearest(1);          // В dest[1] находится цветное
    // --------------------

    // Клешинг
    //m.clashing_v2();
    m.clashing();

    // Обновление
    m.scr2source();

    // Вывод данных
    m.save_ppm(argc > 2 ? argv[2] : "result_output.ppm");
    m.save_scr(argc > 3 ? argv[3] : "result_output.scr");

    return 0;
}
