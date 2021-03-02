class ppm {

public:

    int     width, height;
    uint *  data;

    ppm();
    ppm(int, int);
    ppm(const char*);
    ~ppm();

    /** Загрузка файла */
    int load(const char*);
    
    /** Получение точки */
    uint point(int, int);
};
