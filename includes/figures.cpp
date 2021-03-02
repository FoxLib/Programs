class figures {

public:

    // fs = 256.0 - размер дискеты 256 пикселей
    void diskette(image* img, int x, int y, float fs) {

        // Нормализация до 320 
        fs /= 320.0;

        // Сама дискета
        img->block       (x, y, x + fs*320.0, y + fs*320.0, 0x444048);
        img->outline     (x, y, x + fs*320.0, y + fs*320.0, 0);

        // Кружок диска
        img->circlefill  (x + fs*160.0, y + fs*160.0, fs*50.0, 0x606060);
        img->circlefill  (x + fs*160.0, y + fs*160.0, fs*43.0, 0x303030);
        img->circlefill  (x + fs*160.0, y + fs*160.0, fs*36.0, 0xffffff);

        img->circle      (x + fs*160.0, y + fs*160.0, fs*50.0, 0);
        img->circle      (x + fs*160.0, y + fs*160.0, fs*43.0, 0x808080);

        // Область чтения
        img->circlefill  (x + fs*160.0, y + fs*240.0, fs*12.0, 0x606060);
        img->circlefill  (x + fs*160.0, y + fs*290.0, fs*12.0, 0x606060);
        img->circle      (x + fs*160.0, y + fs*240.0, fs*12.0, 0x303030);
        img->circle      (x + fs*160.0, y + fs*290.0, fs*12.0, 0x303030);
        
        img->block       (x + fs*(160.0-12.0), y + fs*240.0, x + fs*(160.0+12.0), y + fs*290.0, 0x606060);
        img->line        (x + fs*(160.0-12.0), y + fs*240.0, x + fs*(160.0-12.0), y + fs*290.0, 0x303030);
        img->line        (x + fs*(160.0+12.0), y + fs*240.0, x + fs*(160.0+12.0), y + fs*290.0, 0x303030);

        // Второй кружок (индексное отверстие)
        img->circlefill  (x + fs*220.0, y + fs*180.0, fs*5.0, 0x606060);
        img->circle      (x + fs*220.0, y + fs*180.0, fs*5.0, 0x303030);

        img->block       (x + fs*16.0, y + fs*16.0, x + fs*128.0, y + fs*64.0, 0x509030);
        img->outline     (x + fs*16.0, y + fs*16.0, x + fs*128.0, y + fs*64.0, 0x204010);

        for (int i = 24; i < 64; i += 16)
            img->line(x + fs*24.0, y + fs*(float)i, x + fs*(128.0-8.0), y + fs*(float)i, 0x80c050);

        img->transparent(0xffffff, 8);
    }

};
