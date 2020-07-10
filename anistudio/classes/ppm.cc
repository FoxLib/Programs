ppm::ppm() {
}

ppm::ppm(int w, int h) {

    width  = w;
    height = h;
    
    data = (uint*)malloc(sizeof(uint) * w * h);
};

ppm::ppm(const char* filename) {

    data = NULL;
    load(filename);
};

ppm::~ppm() {

    if (data != NULL) free(data);
}

// Загрузка файла
int ppm::load(const char* filename) {

    char* r;
    char  bf[256];
    unsigned char rgb[3];
    int   k = 0;
    
    FILE* fp = fopen(filename, "rb");

    if (fp) {

        r = fgets(bf, 256, fp);
        if (strcmp(r, "P6\n") == 0) {

            r = fgets(bf, 256, fp);
            if (r[0] == '#')
            r = fgets(bf, 256, fp);

            sscanf(r, "%d %d", &width, &height);

            if (data != NULL) free(data);

            // Инициализация
            data = (uint*)malloc(sizeof(uint) * width * height);

            // Загрузка
            for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                
                fread(rgb, 1, 3, fp);
                data[k++] = (rgb[1]<<16) + (rgb[2]<<8) + (rgb[0]);
            }
            
        } else {

            return 2;
        }

        return 1;
    }

    fclose(fp);
    return 0;
}

/** Получение точки */
uint ppm::point(int x, int y) {

    if (x >= 0 && x < width && y >= 0 && y < height) {
        return data[width*y + x];
    }

    return 0;
}
