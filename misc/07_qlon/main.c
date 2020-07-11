#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define FIELDSIZE 1000
#define NUMBER 5

struct item {

    float x;
    float y;
    float q;
    float force_x;
    float force_y;
};

int main(int argc, char* argv[]) {

    int i, j;

    float k = 8.9875517873681764e9;

    struct item q[NUMBER];

    /*
     * Генерация случайных значений
     */

    srand(102452);
    for (i = 0; i < NUMBER; i++) {
        q[i].x = FIELDSIZE * (float)rand() / (float)RAND_MAX; // x = 0..FIELDSIZE
        q[i].y = FIELDSIZE * (float)rand() / (float)RAND_MAX; // y = 0..FIELDSIZE
        q[i].q = (((float)rand() / (float)RAND_MAX) * 4 - 2) / 1000;   // q = -0.002 ... 0.002
    }

    /*
     * Вычисление нового поля
     */

    for (i = 0; i < NUMBER; i++) {
        
        float fx = 0, fy = 0;

        for (j = 0; j < NUMBER; j++) {

            if (i == j) {
                continue;
            }
            
            float dx = (q[i].x - q[j].x);
            float dy = (q[i].y - q[j].y);
            
            float r2 = dx*dx + dy*dy;
            float r  = sqrt(r2);
            float f  = (k / r2) * (q[i].q * q[j].q);
            
            // можно тут вывести силу f для зарядов q[i] и q[j]
            
            fx += dx / r * f;
            fy += dy / r * f;        
        }
        
        q[i].force_x = fx;
        q[i].force_y = fy;
    }
    
    /*
     * Вывод на экран вектора движения каждого заряда: F(x,y)
     */
     
    for (i = 0; i < NUMBER; i++) {
        
        printf("%d [%f, %f]\n", i, q[i].force_x, q[i].force_y);
    }

    return 0;
}

