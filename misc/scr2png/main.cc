#include <stdio.h>
#include <stdlib.h>

#include "lodepng.h"
#include "lodepng.cc"

int factor;
int width, height;

unsigned char  memory[6912];
unsigned int*  image;

uint get_color(int color) {

    switch (color) {
        case 0:  return 0x000000;
        case 1:  return 0x0000c0;
        case 2:  return 0xc00000;
        case 3:  return 0xc000c0;
        case 4:  return 0x00c000;
        case 5:  return 0x00c0c0;
        case 6:  return 0xc0c000;
        case 7:  return 0xc0c0c0;
        case 8:  return 0x000000;
        case 9:  return 0x0000FF;
        case 10: return 0xFF0000;
        case 11: return 0xFF00FF;
        case 12: return 0x00FF00;
        case 13: return 0x00FFFF;
        case 14: return 0xFFFF00;
        case 15: return 0xFFFFFF;
    }

    return 0;
};

// Обновить 8 бит
void update_charline(int address, int flash_state) {

    int Ya = (address & 0x0700) >> 8;
    int Yb = (address & 0x00E0) >> 5;
    int Yc = (address & 0x1800) >> 11;

    int y = Ya + Yb*8 + Yc*64;
    int x = address & 0x1F;

    int byte    = memory[ address ];
    int attr    = memory[ 0x1800 + x + ((address & 0x1800) >> 3) + (address & 0xE0) ];
    int bgcolor = (attr & 0x38) >> 3;
    int frcolor = (attr & 0x07) + ((attr & 0x40) >> 3);
    int flash   = (attr & 0x80) ? 1 : 0;
    int bright  = (attr & 0x40) ? 8 : 0;

    for (int j = 0; j < 8; j++) {

        int  pix = (byte & (0x80 >> j)) ? 1 : 0;

        // Если есть атрибут мерация, то учитывать это
        uint clr = bright | ((flash ? (pix ^ flash_state) : pix) ? frcolor : bgcolor);

        // Вывести пиксель
        clr = get_color(clr);
        clr = ((clr & 0xff) << 16) | ((clr & 0xff00)) | ((clr & 0xff0000)>>16);

        for (int a = 0; a < factor; a++)
        for (int b = 0; b < factor; b++)
            image[factor*(8*x+j)+a + width*(factor*y+b)] = 0xff000000 | clr;
    }
}

int main(int argc, char** argv) {

    if (argc < 3) { printf("scr2png file.scr vulcan.png [1,2,..]\n"); return 1; }

    factor = 1;
    width  = 256;
    height = 192;

    if (argc >= 4) {

        sscanf(argv[3], "%d", & factor);
        width  *= factor;
        height *= factor;
    }

    image = (unsigned int*) malloc(4*width*height);

    FILE* fp = fopen(argv[1], "rb+");
    fread(memory, 1, 6912, fp);
    fclose(fp);

    for (int i = 0; i < 6144; i++) update_charline(i, 0);

    // Кодирование изображения
    unsigned error = lodepng_encode32_file(argv[2], (unsigned char*)image, width, height);
    if (error) printf("error %u: %s\n", error, lodepng_error_text(error));

    return 0;
}
