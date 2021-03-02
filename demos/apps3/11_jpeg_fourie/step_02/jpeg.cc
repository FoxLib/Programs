#include "jpeg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

jpeg::jpeg() {    

    canvas = NULL;
    ycr = NULL;
}

jpeg::~jpeg() {
    if (canvas) free(canvas);
    if (ycr) free(ycr);
}

// Преобразование в волные коэффициенты
void jpeg::DCT1(double v[], double t[]) {

    double dk = M_PI/(2.0*N);
    for (int i = 0; i < N; i++) {

        double sum = 0.0;
        for (int n = 0; n < N; n++) {

            double tm = cos((2.0*(double)n + 1.0)*(double)i*dk);
            sum += v[n] * tm; 
        }

        t[i] = C(i) * sum;
    }
}

// Преобразование из волновых коэффициентов в изображение
void jpeg::IDCT1(double v[], double t[]) {

    double dk = M_PI/(2.0*N);
    for (int i = 0; i < N; i++) {

        double sum = 0.0;
        for (int n = 0; n < N; n++) {

            double tm = cos((2.0*(double)i + 1.0)*(double)n*dk);
            sum += C(n) * t[n] * tm;
        }

        v[i] = sum;
    }
}

// Загрузка PPM-файла в память
void jpeg::LoadPPM(const char* filename) {

    char input[256];
    int  depth;

    FILE* fp = fopen(filename, "rb");

    if (fp) {

        fgets(input, 256, fp); // P6
        if (strcmp(input, "P6\n") == 0) {
        
            while (1) {

                // Читать пока будут комментарии 
                fgets(input, 256, fp); 

                if (input[0] != '#') {
                    sscanf(input, "%d %d", & width, & height);
                    break;
                }
            }

            fgets(input, 256, fp);
            sscanf(input, "%d", & depth);

            if (depth != 255) {

                printf("Проблемы с форматом. Глубина не 255\n"); 
                fclose(fp);
                exit(1);
            }

            // Чтение бинарных данных
            canvas = (RGB*)malloc(sizeof(RGB) * width * height);
            fread(canvas, sizeof(RGB), width * height, fp);

            // Преобразование в новое цветовое пространство
            ToYCR();

        } else {

            printf("Проблемы с форматом. P6 не найден\n"); 
            fclose(fp);
            exit(1);
        }        

        fclose(fp);

    } else {

        printf("Файла не существует: %s\n", filename); 
        exit(1);
    }
}

// Преобразование RGB -> YCR
void jpeg::ToYCR() {

    int i, j;

    if (ycr) free(ycr);
    ycr = (YCR*)malloc(sizeof(YCR) * width * height);

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) { 

            int k = i*width + j;
            RGB pixel = canvas[k];

            float r = (float)pixel.r,
                  g = (float)pixel.g,
                  b = (float)pixel.b;

            ycr[k].Y  =  0.299*r  + 0.587*g  + 0.114*b;
            ycr[k].Cb = -0.1686*r - 0.3313*g + 0.5*b    + 128.0;
            ycr[k].Cr =     0.5*r - 0.4187*g - 0.0813*b + 128.0;
        }
    }
}

// Получение пикселя из canvas
unsigned int jpeg::GetPixel(int x, int y) {

    if (x >= 0 && y >= 0 && x < width && y < height) {

        RGB pixel = canvas[y*width + x];
        return (pixel.r<<16) + (pixel.g<<8) + pixel.b;
    }

    return 0;
}

// Получение YCR по x, y
YCR jpeg::GetYCR(int x, int y) {

    if (x >= 0 && y >= 0 && x < width && y < height) {
        return ycr[y*width + x];
    }

    return {0, 0, 0};
}

int jpeg::GetWidth() { return width; }
int jpeg::GetHeight() { return height; }

// Получение значения по rgb
unsigned int jpeg::GetRGB(unsigned char r, unsigned char g, unsigned char b) {
    return (r << 16) + (g << 8) + b;
}