/*
 * Компиляция
 * После файла .cpp добавить слово "-ljpeg"
 */

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

METHODDEF(void) my_error_exit (j_common_ptr cinfo)
{
    my_error_ptr myerr = (my_error_ptr) cinfo->err;
    (*cinfo->err->output_message) (cinfo);
    longjmp(myerr->setjmp_buffer, 1);
}

/*
 * Класс для работы с Jpeg-файлами
 */

class JPEG {

protected:

    int width;
    int height;
    int filesize;

    unsigned char* data;   // Входящие данные
    unsigned char* output; // Исходящие данные

public:

    // Ничего
    JPEG() {

        width  = height = 0;
        data   = NULL;
        output = NULL;
    }

    ~JPEG() {

        if (data != NULL) free(data);
        if (output != NULL) free(output);
    }

    // Конструктор класса
    JPEG(unsigned int w, unsigned int h, unsigned int rgb) {

        width    = w;
        height   = h;
        data     = (unsigned char*)malloc(w * h * 3);
        output   = NULL;

        int i, j = 0;

        for (i = 0; i < w * h; i++)
        {
            data[j]   = (rgb >> 16) & 0xff;
            data[j+1] = (rgb >> 8) & 0xff;
            data[j+2] = (rgb) & 0xff;

            j += 3;
        }
    }

    /*
     * Чтение изображения JPEG в память
     * Каждый раз после чтения создается новая область в памяти
     */

    int load(const char* filename) {

        unsigned char a,r,g,b;
        int      x;
        struct   jpeg_decompress_struct cinfo;
        struct   my_error_mgr jerr;

        FILE * infile;           // source file
        JSAMPARRAY pJpegBuffer;  // Output row buffer
        int row_stride;          // physical row width in output buffer

        // Невозможно открыть файл
        if ((infile = fopen(filename, "rb")) == NULL) {
            return 2;
        }

        jerr.pub.error_exit = my_error_exit;

        cinfo.err = jpeg_std_error(&jerr.pub);
        jpeg_create_decompress(&cinfo);
        jpeg_stdio_src(&cinfo, infile);

        (void) jpeg_read_header(&cinfo, TRUE);
        (void) jpeg_start_decompress(&cinfo);

        width  = cinfo.output_width;
        height = cinfo.output_height;

        // Инициализация памяти для изображения
        unsigned char* pDummy = (unsigned char*)malloc(width * height * 3);

        // Указатель на начало памяти
        data = pDummy;

        // Не памяти для создания Jpeg-файла
        if (!pDummy) {
            return 1;
        }

        row_stride  = width * cinfo.output_components ;
        pJpegBuffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

        while (cinfo.output_scanline < cinfo.output_height)
        {
            (void) jpeg_read_scanlines(&cinfo, pJpegBuffer, 1);
            for (x = 0; x < width; x++)
            {
                a = 0; // alpha value is not supported on jpg
                r = pJpegBuffer[0][ cinfo.output_components*x ];

                if (cinfo.output_components > 2)
                {
                    g = pJpegBuffer[0][cinfo.output_components*x + 1];
                    b = pJpegBuffer[0][cinfo.output_components*x + 2];
                }
                else
                {
                    g = r;
                    b = r;
                }

                *(pDummy++) = r;
                *(pDummy++) = g;
                *(pDummy++) = b;
            }
        }

        fclose(infile);

        (void) jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);

        return 0;
    }

    /*
     * Запись JPEG из памяти (3 байта на точку)
     * Создается область в памяти
     */

    JPEG* make(int quality) {

        // Упаковка в JPEG
        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr jerr;

        cinfo.err = jpeg_std_error(&jerr);

        // Освобождение памяти перед созданием
        if (output != NULL) free(output);

        // Инициализация памяти
        output = (unsigned char *)malloc(3 * width * height);

        long unsigned int mem_size;

        jpeg_create_compress(&cinfo);
        jpeg_mem_dest(&cinfo, &output, &mem_size);

        cinfo.image_width      = width;
        cinfo.image_height     = height;
        cinfo.input_components = 3;
        cinfo.in_color_space   = JCS_RGB;

        jpeg_set_defaults(&cinfo);
        jpeg_set_quality(&cinfo, quality, TRUE);
        jpeg_start_compress(&cinfo, TRUE);

        JSAMPROW row_pointer[1];
        int      row_stride;

        row_stride = cinfo.image_width * 3;
        while (cinfo.next_scanline < cinfo.image_height)
        {
            row_pointer[0] = (JSAMPLE *)(data + cinfo.next_scanline * row_stride);
            jpeg_write_scanlines(&cinfo, row_pointer, 1);
        }

        jpeg_finish_compress(&cinfo);
        jpeg_destroy_compress(&cinfo);

        // mem_size содержит количество свободной памяти
        filesize = mem_size;

        return this;
    }

    /*
     * Записать данные из памяти
     */

    void write(const char* filename) {

        FILE* f = fopen(filename, "wb");
        fwrite(output, 1, filesize, f );
        fclose(f);
    }

    /*
     * Рисование
     */

     unsigned char* get_data() {
        return data;
     }

     void put(unsigned int x, unsigned int y, unsigned int rgb) {

          unsigned int j = (y*width + x) * 3;
          data[j]   = (rgb >> 16) & 0xff;
          data[j+1] = (rgb >> 8) & 0xff;
          data[j+2] = (rgb) & 0xff;

     }
};
