#ifndef ESCAPE_JPEG_H
#define ESCAPE_JPEG_H

typedef struct JPEG_file
{
    int width;
    int height;

    // Размер выходного файла
    unsigned long int filesize;

    // Изображение в формате RGB.
    // Порядок следования байтов R,G,B
    void* data;

    // Код ошибки
    int error;

} JPEG_file;

#endif