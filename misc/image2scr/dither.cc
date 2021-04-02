#include <math.h>

#define MAX_X 256
#define MAX_Y 192

struct RGB { float r, g, b; };

class DITHER {

protected:

    struct RGB  palette[16];

    int  width, height, depth;
    char buf[256];

    struct RGB src[MAX_X][MAX_Y];
    struct RGB source[MAX_X][MAX_Y];
    int        dest  [MAX_X][MAX_Y];

    // Поиск
    int search_nearest_gray(struct RGB needle) {

        float G = (needle.r + needle.g + needle.b)/3;
        return (G < 128) ? 0 : 15;
    }

    // Найти ближайший цвет
    int search_nearest(struct RGB needle) {

        float dist_min = 4*pow(256, 2);
        int   dist_col = 0;

        // dospal
        for (int i = 0; i < depth; i++) {

            // Сравниваемый цвет
            struct RGB cl = palette[i];

            // Дистанция между точками (r'-r)^2 + (g'-g)^2 + (b'-b)^2
            float dist = pow(needle.r - cl.r, 2) +
                         pow(needle.g - cl.g, 2) +
                         pow(needle.b - cl.b, 2);

            if (dist < dist_min) {
                dist_min = dist;
                dist_col = i;
            }
        }

        return dist_col;
    }

public:

    DITHER() {

        depth = 16;

        palette[0].r  = 0x00; palette[0].g = 0x00; palette[0].b = 0x00;
        palette[1].r  = 0x00; palette[1].g = 0x00; palette[1].b = 0x99;
        palette[2].r  = 0x00; palette[2].g = 0x99; palette[2].b = 0x00;
        palette[3].r  = 0x00; palette[3].g = 0x99; palette[3].b = 0x99;
        palette[4].r  = 0x99; palette[4].g = 0x00; palette[4].b = 0x00;
        palette[5].r  = 0x99; palette[5].g = 0x00; palette[5].b = 0x99;
        palette[6].r  = 0x99; palette[6].g = 0x99; palette[6].b = 0x00;
        palette[7].r  = 0x99; palette[7].g = 0x99; palette[7].b = 0x99;

        palette[8 ].r = 0x00; palette[8 ].g = 0x00; palette[8 ].b = 0x00;
        palette[9 ].r = 0x00; palette[9 ].g = 0x00; palette[9 ].b = 0xff;
        palette[10].r = 0x00; palette[10].g = 0xff; palette[10].b = 0x00;
        palette[11].r = 0x00; palette[11].g = 0xff; palette[11].b = 0xff;
        palette[12].r = 0xff; palette[12].g = 0x00; palette[12].b = 0x00;
        palette[13].r = 0xff; palette[13].g = 0x00; palette[13].b = 0xff;
        palette[14].r = 0xff; palette[14].g = 0xff; palette[14].b = 0x00;
        palette[15].r = 0xff; palette[15].g = 0xff; palette[15].b = 0xff;
    }

    int load_ppm(const char* filename) {

        FILE* fp = fopen(filename, "rb");

        fgets (buf, 256, fp); // P6
        fgets (buf, 256, fp); // # Comment
        fgets (buf, 256, fp); // W H
        sscanf(buf, "%d %d", &width, &height);
        fgets (buf, 256, fp); // 255

        for (int y = 0; y < height; y++)
        for (int x = 0; x < width;  x++) {

            fread(buf, 1, 3, fp);

            float gray = (buf[0] + buf[1] + buf[2]) / 3;

            src[x][y].b = (unsigned char) buf[0];
            src[x][y].g = (unsigned char) buf[1];
            src[x][y].r = (unsigned char) buf[2];
        }

        fclose(fp);
    }

    void save_ppm(const char* filename) {

        FILE* fp = fopen(filename, "wb+");

        sprintf(buf, "%d %d\n", width, height);

        fputs("P6\n", fp);
        fputs("# Created by IrfanView\n", fp);
        fputs(buf, fp);
        fputs("255\n", fp);

        for (int y = 0; y < height; y++)
        for (int x = 0; x < width;  x++) {

            buf[0] = source[x][y].b;
            buf[1] = source[x][y].g;
            buf[2] = source[x][y].r;
            fwrite(buf, 1, 3, fp);
        }
        fclose(fp);
    }

    // Процедура дизеринга (left, top)-(right, bottom), dith=0 (nearest) 1(dithered)
    void ditherect() {

        // Копировать точки во временное хранение
        for (int y = 0; y < height; y++)
        for (int x = 0; x < width;  x++)
            source[x][y] = src[x][y];

        // Выполнить дизеринг
        for (int y = 0; y < height; y++)
        for (int x = 0; x < width;  x++) {

            struct RGB old = src[x][y];                 // Старые цвета
            int color_id   = search_nearest(old);       // Поиск ближайшего цвета из палитры
            RGB ncl        = palette[color_id];         // Заменить на новый цвет (из палитры)
            source[x][y]   = ncl;                       // Записать приблизительный цвет
            dest[x][y]     = color_id;                  // Полученный индекс цвета

            // Вычисляем ошибку квантования
            RGB quant;

            quant.r = (old.r - ncl.r);
            quant.g = (old.g - ncl.g);
            quant.b = (old.b - ncl.b);

            //   x 7
            // 3 5 1

            // [+1, +0] 7/16
            if (x + 1 < width) {

                source[x + 1][y].r += (quant.r * 7.0/16.0);
                source[x + 1][y].g += (quant.g * 7.0/16.0);
                source[x + 1][y].b += (quant.b * 7.0/16.0);
            }

            // [-1, +1] 3/16
            if (x - 1 >= 0 && y + 1 < height) {

                source[x-1][y+1].r += (quant.r * 3.0/16.0);
                source[x-1][y+1].g += (quant.g * 3.0/16.0);
                source[x-1][y+1].b += (quant.b * 3.0/16.0);
            }

            // [+0, +1] 5/16
            if (y + 1 < height) {

                source[x][y+1].r += (quant.r * 5.0/16.0);
                source[x][y+1].g += (quant.g * 5.0/16.0);
                source[x][y+1].b += (quant.b * 5.0/16.0);
            }

            // [+1, +1] 1/16
            if (x + 1 < width && y + 1 < height) {

                source[x+1][y+1].r += (quant.r * 1.0/16.0);
                source[x+1][y+1].g += (quant.g * 1.0/16.0);
                source[x+1][y+1].b += (quant.b * 1.0/16.0);
            }
        }
    }

    void clashing() {

        for (int y = 0; y < 24; y++) {
            for (int x = 0; x < 32; x++) {

                int color_table[16];

                // Очистка цветовой таблицы
                for (int i = 0; i < 16; i++)
                    color_table[i] = 0;

                // Сборка цветов
                for (int i = 0; i < 8; i++)
                for (int j = 0; j < 8; j++) {
                    color_table[ dest[x*8+j][y*8+i] & 15 ]++;
                }

                // Дурная сортировка
                int max1 = -1,   max2 = -1,
                    max1_id = 0, max2_id = 0;

                // Цвет фона
                for (int i = 0; i < 16; i++) {
                    if (color_table[i] > max1) {
                        max1 = color_table[i];
                        max1_id = i;
                    }
                }

                // Цвет не фона
                int from = 0, to = 8;

                for (int i = 0; i < 16; i++) {
                    if (i == max1_id) continue;
                    if (color_table[i] > max2) {
                        max2 = color_table[i];
                        max2_id = i;
                    }
                }

                // Выбрать карту цвета
                if ((max1_id&8) != (max2_id&8)) {
                    max2_id = (max2_id & 7) | (max1_id&8);
                }

                struct RGB back = palette[max1_id];
                struct RGB attr = palette[max2_id];

                // Обновление новых атрибутов
                for (int i = 0; i < 8; i++)
                for (int j = 0; j < 8; j++) {

                    struct RGB cl = source[8*x+j][8*y+i];

                    float dist1 = (cl.r-back.r)*(cl.r-back.r) +
                                  (cl.g-back.g)*(cl.g-back.g) +
                                  (cl.b-back.b)*(cl.b-back.b);

                    float dist2 = (cl.r-attr.r)*(cl.r-attr.r) +
                                  (cl.g-attr.g)*(cl.g-attr.g) +
                                  (cl.b-attr.b)*(cl.b-attr.b);

                    // Что ближе?
                    source[8*x+j][8*y+i] = dist1 < dist2 ? back : attr;
                }


            }
        }
    }
};
