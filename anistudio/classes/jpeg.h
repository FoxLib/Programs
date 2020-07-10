#ifndef __FILECLASS_JPEGH
#define __FILECLASS_JPEGH

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <setjmp.h>

struct my_error_mgr {
    struct  jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;

class jpeg {

public:

    int  width, height;

    unsigned char   r, g, b;
    unsigned char * data;               // Изображение
    unsigned char * output;             // Исходящие данные (файл)

    JSAMPARRAY          pJpegBuffer;    // Выходной временный буфер JPEG (по 1 линии)
    int                 row_stride;     // Физическая длина линии в выходном буфере
    long unsigned int   mem_size;       // Количество занимаемой памяти изображением

    /** Конструктор */
    jpeg();

    /** Деструктор */
    ~jpeg();

    /** Создание пустого изображения */
    jpeg(int, int);

    /** Создание из файла */
    jpeg(const char*);

    /** Инициализация памяти */
    void init();

    /** Создать область в памяти */
    void create(int, int);

    /** Запись JPEG из памяти (3 байта на точку): quality - качество */
    int make(int quality);

    /** Загрузка файла jpeg в память */
    int load(const char* filename);

    /** Сохранить jpeg на диске: должен быть подготовлен с помощью make */
    void save(const char* filename);

    /** Более общее сохранение */
    void save(const char* filename, int quality);

    /** Получение значения цвета в INT */
    uint point(int x, int y);

    /** Записать точку */
    void pset(int x, int y, int color);
};

#endif
