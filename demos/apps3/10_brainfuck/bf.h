#include <stdio.h>

class BF {

private:

    int*            cpu;            // Память под зверь-машину
    int             size;           // Размер программы
    int             maxsize;        // Максимальный размер
    unsigned char*  program;        // Программный код
    FILE*           input;          // Входящий поток сознания

    int     i;          // Текущий указатель на cpu[i]
    int     pc;         // Указатель программы

    int     stepcount;

public:

    BF();
    ~BF();

    void            accept_input();
    void            load_program(const char*);
    unsigned char   step();
};
