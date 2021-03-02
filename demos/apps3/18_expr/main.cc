#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Необходимо для forward-доступа из expr3
int expr1(char e[], int& i, int l);

// Удаление пробелов
void avoid_spaces(char e[], int& i, int l) {
    while (i < l && e[i] == ' ') i++;
}

// Принадлежит множеству [A-Za-z_]
int is_alpha(char ch) {

    return (ch >= 'A' && ch <= 'Z') ||
           (ch >= 'a' && ch <= 'z') ||
            ch == '_' ? 1 : 0;
}

// Выражение третьего уровня (числа, символы)
int expr3(char e[], int& i, int l) {

    avoid_spaces(e, i, l);

    int v = 0;

    // -------------------
    // Скобки?
    // -------------------
    if (e[i] == '(') {

        i++; v = expr1(e, i, l);
        if (e[i] == ')') i++; // else error()
    }
    // -------------------
    // Это переменная?
    // -------------------
    else if (is_alpha(e[i])) {

        // @todo Найти переменную
        while (is_alpha(e[i])) { i++; }
        v = 0;
    }
    // -------------------
    // Получение числа
    // -------------------
    else {

        while (i < l) {

            if (e[i] >= '0' && e[i] <= '9')
                 v = 10*v + (e[i] - '0');
            else break;

            i++;
        }
    }

    avoid_spaces(e, i, l);
    return v;
}

// Выражения второго уровня (div, mul)
int expr2(char e[], int& i, int l) {

    int a, b;

    // Левая часть
    a = expr3(e, i, l);

    // Поиск по цепочке
    while (i < l) {

        if (e[i] == '*') {
            i++; b = expr3(e, i, l); a *= b;
        } else if (e[i] == '/') {
            i++; b = expr3(e, i, l); a /= b;
        } else {
            break;
        }
    }

    return a;
}

// Выражения первого уровня (plus, minus)
int expr1(char e[], int& i, int l) {

    int a, b;

    // Левая часть
    a = expr2(e, i, l);

    // Поиск по цепочке
    while (i < l) {

        if (e[i] == '+') {
            i++; b = expr2(e, i, l); a += b;
        } else if (e[i] == '-') {
            i++; b = expr2(e, i, l); a -= b;
        } else {
            break;
        }
    }

    return a;
}

// Функция верхнего уровня
int expr(char e[]) {

    int i = 0;
    return expr1(e, i, strlen(e));
}

// Главная функция
int main(int argc, char* argv[]) {

    char ein[] = "2 + (10 - 3*6)/4 - 7";

    printf("\n=%d\n", expr(ein));
    return 0;
}
