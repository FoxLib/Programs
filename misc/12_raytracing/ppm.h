#include <stdlib.h>
#include <stdio.h>

struct RGB {
    unsigned char r, g, b;
};

class ppm {

protected:

    int width, height;
    RGB* canvas;

public:

    ppm(int, int);
    ~ppm();

    void pset(int, int, RGB);
    void save(const char*);
};