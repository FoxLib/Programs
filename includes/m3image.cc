
struct __attribute__((__packed__)) RGB {
    unsigned char b;
    unsigned char r;
    unsigned char g;
};

// Изображение или текстура
// ---------------------------------------------------------------------
class m3image {
protected:

    RGB* image;
    int width, height, depth;

public:

    // Загрузка PPM
    m3image(const char* filename) {

        char buf[256];

        width  = 0;
        height = 0;
        depth  = 0;
        image  = NULL;

        FILE* fp = fopen(filename, "rb");
        if (fp) {

            char* m = fgets(buf, 255, fp);

            // Валидный формат
            if (m && (strcmp(buf, "P6\n") == 0)) {

                while (1) {

                    // Считывается
                    if (fgets(buf, 255, fp)) {

                        // Пока не будет # комментарии
                        if (buf[0] != '#') {

                            // Читается w, h
                            sscanf(buf, "%d %d", &width, &height);

                            // Долбанный warn_unused_result
                            if (fscanf(fp, "%d", &depth)) {

                                image = (struct RGB*) malloc(3 * width * height);
                                if (fread(image, 3, width*height, fp)) {
                                    // ok
                                }
                            }

                            break;
                        }

                    } else break;
                }

            }

            fclose(fp);
        }
    }

    // Получение точки
    RGB point(int x, int y) {

        RGB cl = {0, 0, 0};
        if (width && height) {

            if (x < 0) x += ((-x / width)  + 1) * width;
            if (y < 0) y += ((-y / height) + 1) * height;

            x %= width;
            y %= height;
            cl = image[x + y*width];
        }

        return cl;
    }

    // Алиас для [0..1], [0..1]
    RGB point(float x, float y) {
        return point((int)(x*width), (int)(y*height));
    }

    ~m3image() {
        if (image) free(image);
    }
};
