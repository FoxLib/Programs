#include <number.h>

// Ширина числа
number::number() {
    wide = 16;
}

// Присваивание нового значения
void number::operator=(double t) {
 
    int b;

    if (t < 0) {
        sign = 1;
        t = -t;
    } else {
        sign = 0;
    }

    // Нулевой элемент равен целому числу от 0 до 255
    num[0] = t; t -= (double)num[0];

    // Выстраивание дробей
    for (int i = 1; i < wide; i++) 
    for (int j = 0; j < 8; j++) {

        b = 1 << (7 - j);
        num[i] &= (0xff ^ b);
        t *= 2.0;
        if (t >= 1.0) { t -= (int)t; num[i] |= b; }
    }
}

// Получение доступа
int number::operator[](int n) {
    if (n < 0 || n >= wide) return 0;
    return num[n];
}

// Сохранение
void number::setnum(int i, int a) {
    num[i] = a;
}

// Сравнение числа

// Сложение чисел
number number::operator+(number op2) {

    number R;

    R.sign = 0;

    // if (sign == 0 && op2.sign() == 0) { }

    int carry = 0;
    for (int i = wide - 1; i >= 0; i--) {
        
        int n = num[i] + op2[i] + carry;
        if (n > 255) { n -= 256; carry = 1; } else carry = 0;
        R.setnum(i, n);
    }

    return R;
}

// Печать бинарного числа
void number::printbin() {

    if (sign > 0) printf("-");
    for (int i = 0; i < wide; i++) {
        if (i == 1) printf(".");
        for (int j = 7; j >= 0; j--) {
            printf("%c", (num[i] & (1<<j)) ? '1' : '0');
        }
    }
}