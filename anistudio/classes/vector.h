struct vec2i {

    int x;
    int y;
};

struct vec2f {

    double x;
    double y;
};

struct vec3f {

    double x;
    double y;
    double z;
};

// Вершина и uv-маппинг
struct vec5f {

    double x, y, z;
    double u, v;
};

// Столкновение
struct TCollide {

    int     cast;       // Было ли?
    double  dist;       // Дистанция по Z (coord.z)
    vec3f   coord;      // Где было столкновение
    vec3f   normal;     // Нормаль в точке столкновения
    vec3f   direct;     // Нормаль к точке (0, 0, 0) от точки столкновения
};
