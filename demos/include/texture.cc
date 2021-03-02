#include <stdlib.h>
#include <jpeg.cc>

class GTexture {

protected:

    int     width;
    int     height;
    GLuint* _data;

public:

    GTexture(int w, int h) {

        width  = w;
        height = h;
        _data   = (GLuint*)(malloc(w * h * sizeof(GLuint)));
    }

    GTexture(const char* filename) {

        jpeg image(filename);

        width  = image.width;
        height = image.height;

        if (width && height) {

            int k = 0;
            _data = (GLuint*)(malloc(width * height * sizeof(GLuint)));

            // Скопировать в память
            for (int y = 0; y < height; y++)
            for (int x = 0; x < width;  x++) {
                _data[y*width + x] = (image.data[k+2] + 256*image.data[k+1] + 65536*image.data[k+0]) | 0xFF000000;
                k += 3;
            }

        } else {
            _data = NULL;
        }
    }

    ~GTexture() {
        if (_data) free(_data);
    }

    // I/O - как на чтение, так и на запись
    GLuint& operator[] (const int index) {
        return _data[index];
    }

    GLuint& operator() (const int x, const int y) {
        return _data[width*y + x];
    }

    GLuint* data() { return _data; }
    int w() { return width; }
    int h() { return height; }
};
