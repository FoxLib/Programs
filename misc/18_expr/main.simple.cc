#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Тестовое вычисление выражений
 */

// Вычисление результата
int expr_action(int r, int v, char a) {

    switch (a) {
        case '*': r = r * v; break;
        case '/': r = r / v; break;
        case '+': r = r + v; break;
        case '-': r = r - v; break;
    }
    return r;
}

// Вычисление умножения и деления последовательно
int expr_muldiv(char* e, int & i, int l) {

    int  r = 1;
    int  v = 0;
    char a = '*';

    // Пропуск пробелов
    while (i < l && e[i] == ' ') i++;

    // Расчет умножения и деления
    while (i < l) {

        // Вычисление цифры
        if (e[i] >= '0' && e[i] <= '9') {
            v = 10*v + (e[i] - '0');
        }
        // Операция умножения или деления
        else if (e[i] == '*' || e[i] == '/') {

            r = expr_action(r, v, a);
            a = e[i];
            v = 0;
        }
        // Уровень выше
        else if (e[i] == '-' || e[i] == '+') {
            break;
        }

        i++;
    };

    // Вычисление результата в конце символов
    return expr_action(r, v, a);
}

// Вычисление выражения
int expr(char* e, int & i) {

    int  v;
    char a = '+';
    int  r = 0;
    int  l = strlen(e);

    while (i < l) {

        v = expr_muldiv(e, i, l);

        // Пропуск пробелов
        while (i < l && e[i] == ' ') i++;

        // Высчитывает с предыдущим значением
        if (e[i] == '-' || e[i] == '+') {

            r = expr_action(r, v, a);
            a = e[i];
        }

        i++;
    }

    // Окончательный пересчет
    return expr_action(r, v, a);
}

int main(int argc, char* argv[]) {

    char ein[] = "2+2*2";

    int i = 0;
    printf("\n\n%s = %d\n", ein, expr(ein, i));
    return 0;
}
