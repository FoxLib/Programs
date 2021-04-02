#include <stdio.h>
#include "dither.cc"

/**
 * Конверсия PPM-файла 256x192 в SCR
 */

int main(int argc, char** argv) {

    DITHER dither;

    if (argc <= 1) { printf("Требуется имя файла\n"); exit(0); }

    dither.load_ppm(argv[1]);
    // --- Нормализация, чтобы цветность картинки не была более 127 и ее средний цвет был 64
    dither.ditherect();
    dither.clashing();
    dither.save_ppm("test.ppm");

    return 0;
}
