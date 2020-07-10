
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
    unsigned char*  data;               // Изображение
    unsigned char*  output;             // Исходящие данные (файл)

    JSAMPARRAY          pJpegBuffer;    // Выходной временный буфер JPEG (по 1 линии)
    int                 row_stride;     // Физическая длина линии в выходном буфере
    long unsigned int   mem_size;       // Количество занимаемой памяти изображением

    jpeg();
    ~jpeg();

    jpeg(int, int);
    jpeg(const char*);

    void init();
    void create(int, int);
    int  make(int quality);
    int  load(const char* filename);
    void save(const char* filename);
    void save(const char* filename, int quality);

    unsigned int point(int x, int y);
    void pset(int x, int y, int color);
};

// Конструктор
jpeg::jpeg() {

    init();
}

// Деструктор
jpeg::~jpeg() {

    if (data != NULL) free(data);
    if (output != NULL) free(output);
}

// Создание пустого изображения
jpeg::jpeg(int w, int h) {

    init();
    create(w, h);
}

// Создание из файла
jpeg::jpeg(const char* filename) {

    init();
    load(filename);
}

/** Инициализация памяти */
void jpeg::init() {

    data   = NULL;
    output = NULL;
    width  = 0;
    height = 0;
}

/** Создать область в памяти */
void jpeg::create(int w, int h) {

    width  = w;
    height = h;

    // Размер области память
    int size = (3 * width * height);

    // А потом выделение области в этой самой памяти
    data = (unsigned char *) malloc(size);

    // И дальше очистка от мусора
    int i; for (i = 0; i < size; i++) data[i] = 0;
}

/** Запись JPEG из памяти (3 байта на точку): quality - качество */
int jpeg::make(int quality) {

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr       jerr;

    cinfo.err = jpeg_std_error(&jerr);

    // Освобождение памяти перед созданием
    if (output != NULL) free(output);

    // Инициализация памяти (3 байта на точку)
    output = (unsigned char *) malloc(3 * width * height);

    jpeg_create_compress(&cinfo);
    jpeg_mem_dest(&cinfo, &output, &mem_size);

    cinfo.image_width      = width;
    cinfo.image_height     = height;
    cinfo.input_components = 3;
    cinfo.in_color_space   = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    JSAMPROW    row_pointer[1];
    int         row_stride;

    row_stride = cinfo.image_width * 3;
    while (cinfo.next_scanline < cinfo.image_height) {

        row_pointer[0] = (JSAMPLE *)(data + cinfo.next_scanline * row_stride);
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    return 1;
}

/** Загрузка файла jpeg в память */
int jpeg::load(const char* filename) {

    int x;

    unsigned char * picture; // Временная

    struct      jpeg_decompress_struct cinfo;  // Структура для JPEG потока
    struct      jpeg_error_mgr         jerr;   // Обработчик ошибок
    FILE *      infile;

    width  = 0;
    height = 0;

    // Попытка прочитать файла
    if ((infile = fopen(filename, "rb")) == NULL) {
        return 1;
    }

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);

    (void) jpeg_read_header(&cinfo, TRUE);
    (void) jpeg_start_decompress(&cinfo);

    // Получение размера JPEG картинки
    width  = cinfo.output_width;
    height = cinfo.output_height;

    // Выделить память под изображение
    data     = (unsigned char *) malloc(3 * width * height);
    picture  = data;

    // Ошибка выделения памяти
    if (!data) {
        return 2;
    }

    row_stride  = width * cinfo.output_components;
    pJpegBuffer = (*cinfo.mem->alloc_sarray)( (j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    // Построчно получить файл в память
    while (cinfo.output_scanline < cinfo.output_height)
    {
        // Прочитать 1 строку изображения из JPEG файла
        (void) jpeg_read_scanlines(&cinfo, pJpegBuffer, 1);

        // Перебрать все пиксели в этой строке
        for (x = 0; x < width; x++) {

            // Сначала получим RED-компоненту
            r = pJpegBuffer[0][ cinfo.output_components*x ];

            // Если изображение цветное (3 компоненты)
            if (cinfo.output_components > 2) {
                g = pJpegBuffer[0][ cinfo.output_components*x + 1 ];
                b = pJpegBuffer[0][ cinfo.output_components*x + 2 ];
            }
            // Иначе изображение монохромное
            else {
                g = r;
                b = r;
            }

            // Записать (r, g, b) - 3 байта
            *( picture++ ) = r;
            *( picture++ ) = g;
            *( picture++ ) = b;
        }
    }

    fclose(infile);
    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    return 0;
}

/** Сохранить jpeg на диске: должен быть подготовлен с помощью make */
void jpeg::save(const char* filename) {

    FILE* fp = fopen(filename, "wb");

    if (fp != NULL) {
        fwrite(output, 1, mem_size, fp);
        fclose(fp);
    }
}

/** Более общее сохранение */
void jpeg::save(const char* filename, int quality) {

    if (make(quality)) {
        save(filename);
    }
}

/** Получение значение цвета в INT */
unsigned int jpeg::point(int x, int y) {

    int addr = 3*(x + y*width);
    return (data[ addr ]<<16) + (data[ addr+1 ]<<8) + data[ addr+2 ];
}

/** Записать точку */
void jpeg::pset(int x, int y, int color) {

    int addr = 3*(x + y*width);

    data[ addr + 0 ] = (color >> 16) & 0xff;
    data[ addr + 1 ] = (color >> 8) & 0xff;
    data[ addr + 2 ] = (color) & 0xff;
}

