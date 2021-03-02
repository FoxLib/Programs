#include <stdio.h>
#include <stdlib.h>

#include "bf.h"

int main(int argc, char* argv[]) {

    BF bf;

    bf.accept_input();
    bf.load_program(argc > 1 ? argv[1] : "app/hello.bf");
    while (bf.step());

    return 0;
}
