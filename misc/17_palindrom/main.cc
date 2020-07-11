#include <stdlib.h>
#include <stdio.h>

int check(long x, long y) {

    int i, d;
    int cnt[10];

    for (i = 0; i < 10; i++) cnt[i] = 0;

    // Уникальность цифр в X
    do {

        d = x % 10;
        x = x / 10;

        if (d == 0) return 0;
        if (cnt[d]) return 0; else cnt[d]++;

    } while (x > 0);

    // Уникальность цифр в Y
    do {

        d = y % 10;
        y = y / 10;
        if (cnt[d] != 1) return 0; else cnt[d]--;
    }
    while (y > 0);

    // Все прошло успешно
    return 1;
}

int main(int argc, char* argv[]) {

    long x, y;

    for (x = 100000; x < 258976452; x++) {

        if (check(x, 7*x)) {
            printf("%li %li\n", x, 7*x);
        }
    }

    return 0;
}
