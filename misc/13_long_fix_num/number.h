#include <stdio.h>

// https://ru.wikipedia.org/wiki/Операторы_в_C_и_C++
class number {
private:
 
    int wide;
    int sign;
    unsigned char num[128];
    unsigned char tmp[256];

public:
    
    number();

    void operator=(double t);
    int  operator[](int);
    number operator+(number);

    void setnum(int, int);
    void printbin();
};