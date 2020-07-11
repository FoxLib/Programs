#ifndef __JPEGLOAD_H
#define __JPEGLOAD_H

#define JPEG_ERROR_NONE         0   // Ошибок нет
#define JPEG_ERROR_NOFILE       1   // Файл не читаемый
#define JPEG_ERROR_NOMEM        2   // Не хватило памяти

// Структура для описания файла из загруженному jpeg
struct jpeg_file_struct
{
    int             width;          // Ширина
    int             height;         // Высота
    unsigned char*  data;           // Здесь находятся данные (картинка)
    int             error;          // Код ошибки

};

// Прототип 
struct jpeg_file_struct loadjpeg(const char*);

#endif
