#include <stdlib.h>
#include <stdio.h>

unsigned char uncompress[ 1024*1024 ]; // Область памяти для распаковки
unsigned char compressed[ 1024*1024 ]; // Загруженное в память значение

struct dict_item {

    int addr;
    int size;
};

// Прочитать из битового потока
int fetch(int ch, int cl, int bl) {

    unsigned int v3, sym;

    // Прочтение нового значение (3 байта макс)
    v3 = compressed[ch] + (compressed[1+ch]*256) + (compressed[2+ch]*65536);

    // Получение байта
    sym = (v3 >> cl) & ((1 << bl) - 1);

    return sym;
}

int main(int argc, char* argv[]) {

    if (argc <= 1) { printf("Нужен аргумент!\n"); return 0; }

    FILE* fp = fopen(argv[1], "r");

    // Загрузка в память (пропуск лидирующих 3 байт)
    fseek(fp, 3, SEEK_SET);
    int l = 0; while (1) { int az = fread(compressed + l, 1, 4096, fp); if (az == 0) break; l += az; }

    // Создаем словарь
    struct dict_item* dict = (struct dict_item*) malloc(sizeof(struct dict_item) * 65536);

    unsigned int sym, din;

    // ----------------------

    int  dl = 0; // Длина словаря
    int  cl = 0; // Младший адрес
    int  ch = 0; // Текущий адрес
    int  bl = 9; // Длина в битах
    int  di = 0; // Адрес на выдачу
    int  da, dz; // Распаковка кодов

    int i, j;

    while (ch < l) {

        // Расчет длины словаря
        if (dl < 0x100) bl = 9;
        else if (dl < 0x300) bl = 10;
        else if (dl < 0x700) bl = 11;
        else if (dl < 0xF00) bl = 12;
        else if (dl < 0x1F00) bl = 13;
        else if (dl < 0x3F00) bl = 14;
        else if (dl < 0x7F00) bl = 15;
        else bl = 16;

        // Считывание
        sym = fetch(ch, cl, bl);

        // К следующим байтам в потоке
        cl += bl; while (cl > 7) { ch++; cl -= 8; }

        // Одиночный символ
        if (sym < 256) {

            printf("%c", sym);

            dict[ dl ].addr = di;   // Указатель на текущий символ
            dict[ dl ].size = 2;    // Словарь содержит 2 значения

            uncompress[ di ] = sym;

            dl++; di++;
        }
        // Очистка словаря
        else if (sym == 256) {

            dl = 0;
            cl = 0;
            ch = (ch & ~0xf) + 0x10;
        }
        // Выгрузка значения
        else {

            // 257 -- это 0-е значение словаря
            din = sym - 257;
            da  = dict[din].addr;
            dz  = dict[din].size;

            // Увеличить +1 к текущему значению словаря
            dict[ dl ].addr = di;
            dict[ dl ].size = dz + 1;
            dl++;

            // Переписать
            for (j = 0; j < dz; j++) {

                sym = uncompress[da++];
                uncompress[di++] = sym;

                printf("%c", sym);
            }
        }
    }

    return 0;
}
