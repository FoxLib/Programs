#include <math.h>

#define N 8.0
#define C(n) ((n == 0) ? 1.0 / sqrt(N) : sqrt(2.0 / N))
#define CC(i,j) ((i && j) ? 1.0/4.0 : 1.0/8.0)

struct RGB {
    unsigned char r, g, b;
};

struct YCR {
    unsigned char Y, Cb, Cr;
};

class jpeg {

protected:

    int  width, height;
    
    RGB* canvas;
    YCR* ycr;

public:

    jpeg();
    ~jpeg();

    void LoadPPM(const char*);

    void DCT1(double [], double []);
    void IDCT1(double [], double []);

    void DCT8(double [8][8], double [8][8]);
    void IDCT8(double [8][8], double [8][8]);

    int  GetWidth();
    int  GetHeight();
    void ToYCR();
    
    unsigned int GetRGB(unsigned char, unsigned char, unsigned char);
    unsigned int GetPixel(int, int);
    YCR GetYCR(int x, int y);
};