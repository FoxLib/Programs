// -ljpeg

#include "load.h"

#include <stdlib.h>
#include <stdio.h>

#include <stddef.h>
#include <jpeglib.h>

// Чтение изображения JPEG в память
// Память создается динамически

struct jpeg_file_struct loadjpeg(const char* jpegfile)
{
    struct   jpeg_file_struct jpeg;    
    
    // Переменные (r, g, b), куда мы будем скидывать временные данные
    unsigned char r, g, b;
    
    // Временные данные
    int      width, height, x;
    
    // Структура для JPEG потока
    struct   jpeg_decompress_struct cinfo;
    
    // Обработчик ошибок
    struct   jpeg_error_mgr         jerr;

    // Описатель входящего файла
    FILE * infile;
    
    // Выходной временный буфер JPEG (по 1 линии)
    JSAMPARRAY pJpegBuffer;
    
    // Физическая длина линии в выходном буфере
    int row_stride;

    // Нет ошибок открытия и чтения файла
    jpeg.error = JPEG_ERROR_NONE;

    // Попытка прочитать файла
    if ((infile = fopen(jpegfile, "rb")) == NULL) 
    {
        jpeg.error = JPEG_ERROR_NOFILE;
        return jpeg;
    }

    // Назначаем обработчик ошибок jpeg (стандартный)
    cinfo.err = jpeg_std_error(&jerr); 
    
    // Создадим структуру декомпрессии 
    jpeg_create_decompress(&cinfo);
    
    // Укажем входной файл (поток), у нас это файл сейчас
    jpeg_stdio_src(&cinfo, infile);

    // Прочесть заголовок JPEG
    (void) jpeg_read_header(&cinfo, TRUE);
    
    // Запуск декомпрессии
    (void) jpeg_start_decompress(&cinfo);

    // Получение размера JPEG картинки
    width  = cinfo.output_width;
    height = cinfo.output_height;

    // Создадим область памяти (4 байта на точку - RGBA)
    unsigned char* picture_area = (unsigned char *) malloc(4 * width * height);
    
    // Этот указатель будет "бегать" по памяти и заполнять данными
    unsigned char* picture_ptr = picture_area;

    // Память не была выделена, что является ошибкой
    if (!picture_area)
    {        
        jpeg.error = JPEG_ERROR_NOMEM;
        return jpeg;
    }

    // Определение длины линии в байтах (cinfo.output_components тут хранится кол-во байт на точку)
    row_stride  = width * cinfo.output_components;
    
    // Создадим структуру временного буфера (инициализация)    
    // JPOOL_IMAGE - флаг, обозначающий сканирование пока не завершиться поток
    
    pJpegBuffer = (*cinfo.mem->alloc_sarray)( (j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    // Построчно получить файл в память (пока текущий сканлайн не будет равен высоте изображения)
    while (cinfo.output_scanline < cinfo.output_height) 
    {
        // Прочитать 1 строку изображения из JPEG файла
        (void) jpeg_read_scanlines(&cinfo, pJpegBuffer, 1);
        
        // Перебрать все пиксели в этой строке
        for (x = 0; x < width; x++) 
        {
            // Сначала получим RED-компоненту
            r = pJpegBuffer[0][ cinfo.output_components*x ];

            // Если изображение цветное (3 компоненты)
            if (cinfo.output_components > 2)
            {
                // То читаем g, b
                g = pJpegBuffer[0][ cinfo.output_components*x + 1 ];
                b = pJpegBuffer[0][ cinfo.output_components*x + 2 ];
            } 
            // Иначе изображение монохромное
            else 
            {
                g = r;
                b = r;
            }

            // Записать (r, g, b, 255) - 4 байта
            *( picture_ptr++ ) = r;
            *( picture_ptr++ ) = g;
            *( picture_ptr++ ) = b;  
            *( picture_ptr++ ) = 255;
        }
    }
    
    // Закрыть файл
    fclose(infile);

    // Закончить декомпрессию
    (void) jpeg_finish_decompress(&cinfo);
    
    // Уничтожить из памяти временные данные 
    jpeg_destroy_decompress(&cinfo);

    // Запись выходных параметров    
    jpeg.width  = width;
    jpeg.height = height;
    jpeg.data   = picture_area;
    jpeg.error  = 0;

    return jpeg;
}
