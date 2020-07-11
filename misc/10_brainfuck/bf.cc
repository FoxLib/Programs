#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bf.h"

BF::BF() {

    maxsize     = 1024*1024;
    cpu         = (int*) malloc(sizeof(int) * maxsize);
    program     = NULL;

    i  = 0;
    pc = 0;
    stepcount = 0;

    memset(cpu, 0, sizeof(int) * maxsize);
}

BF::~BF() {

    free(cpu);
    if (program) free(program);
}

// Поток на вход из стандартного потока
void BF::accept_input() {
    input = stdin;
}

// Загрузка программы
void BF::load_program(const char* filename) {

    FILE* fp = fopen(filename, "rb");
    int   chunk, offset = 0;

    if (fp) {

        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        program = (unsigned char*)malloc(sizeof(unsigned char) * size + 1);
        fseek(fp, 0, SEEK_SET);

        do {

            chunk = fread(program + offset, 1, 512, fp);
            offset += chunk;

        } while (chunk);

        program[offset] = 0;
        fclose(fp);

    } else {

        printf("Файл не найден: %s\n", filename);
        exit(1);
    }
}

// Выполнить шаг инструкции
unsigned char BF::step() {

    stepcount++;

    int bracket;
    unsigned char token = program[pc++];

    switch (token) {

        // Базовые команды
        case '+': cpu[i]++; break;
        case '-': cpu[i]--; break;
        case '<': i = ((i - 1) + maxsize) % maxsize; break;
        case '>': i = ((i + 1) + maxsize) % maxsize; break;
        case '.': printf("%c", cpu[i]); break;
        case ',': cpu[i] = fgetc(input); /* Нет данных */ if (cpu[i] < 0) cpu[i] = 0; break;
        case '[': { // while (cpu[i]) {

            if (cpu[i] == 0) {

                bracket = 1;

                do {

                    token = program[pc++];
                    if      (token == ']') bracket--;
                    else if (token == '[') bracket++;

                } while (bracket);

            }

            break;
        }
        case ']': { // }

            // Если не 0, то найти открывающийся while с нужной вложенностью
            if (cpu[i]) {

                pc--;
                bracket = 1;

                do {

                    token = program[--pc];
                    if      (token == '[') bracket--;
                    else if (token == ']') bracket++;

                } while (bracket);

                pc++;
            }

            break;
        }
    }

    return token;
}
