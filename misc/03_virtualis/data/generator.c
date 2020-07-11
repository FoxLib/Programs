#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define N 256

// 1024x1024 пространство
float fland[ N ][ N ];
int   iland[ N ][ N ];

// [0..1]
float randf(int size) {    
    
    float m = (((float)(rand() % 65536) / 65536.0) - 0.5) * 2;
    float r = 1.0 / size;
    return m * r;        
}

void subdivision(int depth, int x1, int y1, int x2, int y2) {
    
    int mx = (x1 + x2) >> 1;
    int my = (y1 + y2) >> 1;
    
    float v00 = fland[ y1 ][ x1 ];
    float v01 = fland[ y1 ][ x2 ];
    float v10 = fland[ y2 ][ x1 ];
    float v11 = fland[ y2 ][ x2 ];
                
    if (iland[ my ][ mx ] == 0) {
        
        iland[ my ][ mx ] = 1;
        fland[ my ][ mx ] = (v00 + v01 + v10 + v11) / 4.0 + randf(depth);        
    }
    
    if (iland[ y1 ][ mx ] == 0) {
        
        iland[ y1 ][ mx ] = 1;
        fland[ y1 ][ mx ] = (v00 + v01) / 2.0 + randf(depth);
        
    }
        
    if (iland[ y2 ][ mx ] == 0) {
        
        iland[ y2 ][ mx ] = 1;
        fland[ y2 ][ mx ] = (v10 + v11) / 2.0 + randf(depth);
        
    }
    
    if (iland[ my ][ x1 ] == 0) {
        
        iland[ my ][ x1 ] = 1;
        fland[ my ][ x1 ] = (v00 + v10) / 2.0 + randf(depth);
        
    }
    
    if (iland[ my ][ x2 ] == 0) {
        
        iland[ my ][ x2 ] = 1;
        fland[ my ][ x2 ] = (v01 + v11) / 2.0 + randf(depth);
        
    }
        
    if (x2 - x1 > 1 && y2 - y1 > 1) {
           
        subdivision(depth + 1, x1, y1, mx, my);
        subdivision(depth + 1, mx, y1, x2, my);
        subdivision(depth + 1, x1, my, mx, y2);
        subdivision(depth + 1, mx, my, x2, y2);    
    }
    
}

// Сгладить
void smooth() {
    
    int i, j;
    for (i = 1; i < N - 1; i++) {
        for (j = 1; j < N - 1; j++) {
            
            float m = fland[i-1][j] + fland[i+1][j] + fland[i][j-1] + fland[i][j+1];
            
            m /= 4;            
            fland[i][j] = m;
            
        }
    }
}

int main() {
    
    int i, j;
    
    srand (time(NULL));
    
    // Инициализация
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            
            if (i == 0 || j == 0) {
                
                fland[i][j] = 0.0;
                iland[i][j] = 1;
                
            } else {
            
                fland[i][j] = 0.0;
                iland[i][j] = 0;
            }
        }
    }
    
    // Нагенерировать
    subdivision(1, 0, 0, N-1, N-1);    
    
    // Сгладить
    smooth(N);
    smooth(N);
        
    FILE* fp = fopen("landscape/256_00.bin", "w");
    
    float   min = fland[0][0], 
            max = fland[0][0];
            
    // Найти минимальное и максимальное значения для нормализации
    for (i = 0; i < N; i++) {        
        for (j = 0; j < N; j++) {
            if (fland[i][j] < min) min = fland[i][j];
            if (fland[i][j] > max) max = fland[i][j];
        }
    }
    
    // Вывод с нормализацией
    unsigned char bin[N];
    for (i = 0; i < N; i++) {          
        for (j = 0; j < N; j++) {
            bin[j] = (unsigned char)(256 * (fland[i][j] - min) / (max - min));
        }
        
        fwrite(bin, 1, N, fp);        
    }
    
    fclose(fp);
    
    return 0;
}
