#include <stdio.h>

// 0 1 2
// 3 4 5 
// 6 7 8

int desk[9];
int cnt;
int won1, won2;

int test[8][3] = {
    {0, 1, 2}, {0, 3, 6},
    {3, 4, 5}, {1, 4, 7},
    {6, 7, 8}, {2, 5, 8},
    {0, 4, 8}, {2, 4, 6}
};

int won() {

    for (int i = 0; i < 8; i++) {

        int k1 = 0, k2 = 0;
        for (int j = 0; j < 3; j++) {

            int k = desk[ test[i][j] ];
            if (k == 1) k1++;
            if (k == 2) k2++;
        }

        if (k1 == 3) return 1;
        if (k2 == 3) return 2;
    }

    return 0;
}

void pass(int type) {

    int i;
    cnt++;

    /*
    if (cnt < 25) {
        for (i = 0; i < 9; i++) {
            printf("%d ", desk[i]); 
            if ((i % 3) == 2 && i < 8) printf("\n");
        } 
        printf("= %d\n\n", won());
    }
    */

    for (i = 0; i < 9; i++) {

        if (desk[i] == 0) {

            desk[i] = type;

            int w = won();
            if (w == 1) won1++;
            if (w == 2) won2++;

            // проверка выигрышной ситуации
            pass(type == 1 ? 2 : 1);
            desk[i] = 0;
        }
    }
}

int main(int argc, char* argv[]) {

    for (int i = 0; i < 9; i++) desk[i] = 0;

    cnt = 0;

    won1 = won2 = 0;
    pass(1);
    
    printf("%d\n", cnt);
    printf("a=%d, b=%d\n", won1, won2);

    return 0;
}