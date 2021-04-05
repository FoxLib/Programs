#include <stdio.h>
#include "dither.cc"

/**
 * Конверсия PPM-файла 256x192 в SCR
 */

int main(int argc, char** argv) {

    DITHER m;

    if (argc <= 1) { printf("Требуется имя файла\n"); return 1; }
    if (m.load_ppm(argv[1])) { printf("Должен быть файл 256x192\n"); return 1; }

    m.normalize();
    m.dither();
    m.clashing();
    m.save_ppm("result_output.ppm");
    m.save_scr("result_output.scr");

    return 0;
}
