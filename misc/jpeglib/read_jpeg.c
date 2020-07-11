// -ljpeg

#include <stddef.h>
#include <jpeglib.h>
#include "read_jpeg.h"

// Чтение изображения JPEG в память
// ---------------------------------------------------------------------------------------------------
JPEG_file jpeg_load(const char* name)
{
    JPEG_file Jpeg;    
    
    unsigned char r,g,b;
    int      width, height, x;
    struct   jpeg_decompress_struct cinfo;
    struct   jpeg_error_mgr jerr;

    FILE * infile;           // source file 
    JSAMPARRAY pJpegBuffer;  // Output row buffer 
    int row_stride;          // physical row width in output buffer 

    // Нет ошибок открытия и чтения файла
    Jpeg.error = 0;

    if ((infile = fopen(name, "rb")) == NULL) 
    {
        Jpeg.error = 2; // can't open file
        return Jpeg;
    }

    cinfo.err = jpeg_std_error(&jerr); // <<-- cinfo here!
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);

    (void) jpeg_read_header(&cinfo, TRUE);
    (void) jpeg_start_decompress(&cinfo);

    width  = cinfo.output_width;
    height = cinfo.output_height;

    // Инициализация памяти для изображения (RGBA)
    unsigned char* pDummy = (unsigned char *)malloc(width * height * 4);
    unsigned char* pBuffer = pDummy;

    if (!pDummy)
    {        
        Jpeg.error = 1; // NO MEM FOR JPEG CONVERT
        return Jpeg;
    }

    row_stride  = width * cinfo.output_components ;
    pJpegBuffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    while (cinfo.output_scanline < cinfo.output_height) 
    {
        (void) jpeg_read_scanlines(&cinfo, pJpegBuffer, 1);
        for (x = 0; x < width; x++) 
        {
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
            *(pDummy++) = 255;
        }
    }
    
    fclose(infile);

    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    // Запись выходных параметров
    
    Jpeg.width  = width;
    Jpeg.height = height;
    Jpeg.data   = pBuffer;
    Jpeg.error  = 0;

    return Jpeg;
}
