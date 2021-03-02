#include <ppm.h>

ppm::ppm(int w, int h) {

    width  = w;
    height = h;
    canvas = (RGB*)malloc(sizeof(RGB) * w * h);

    for (int i = 0; i < w * h; i++) canvas[i] = {0, 0, 0x40};
}

ppm::~ppm() {
    if (canvas) free(canvas);
}

// Записать точку в память
void ppm::pset(int x, int y, RGB c) {

    if (x >= 0 && y >= 0 && x < width && y < height) {
        canvas[y*width + x] = c;
    }
}

// Сохранить файл на диске
void ppm::save(const char* filename) {

    FILE* fp = fopen(filename, "wb");
    if (fp) {

        char ppmheader[128];
        sprintf(ppmheader, "P6\n%d %d\n255\n", width, height);
        fputs(ppmheader, fp);
        fwrite(canvas, sizeof(RGB), width * height, fp);
        fclose(fp);

    } else {

        printf("Error writing file\n");
        exit(1);
    }
}