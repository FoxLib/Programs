#define M3_ZCLIP 0.001

// Найденная точка пересечения
// ---------------------------------------------------------------------
struct castpoint {

    int     cast;   // Пересечение есть
    float   t;      // t - ближайшее расстояние
    vec2    uv;     // Координаты текстуры
    vec3    coord;  // Координата точки пересечения
    vec3    normal; // Нормаль
    int     ai;     // AutoID объекта
    int     type;   // Тип объекта
};

// Подготовленная структура плоскости
// ---------------------------------------------------------------------
struct m3plane {

    vec3  origin;
    vec3  a, b, c;
    vec3  A, B, C, D;
    float t;
};

// Подготовленная структура сферы
// ---------------------------------------------------------------------
struct m3sphere {

    vec3  center;
    float radius;
};

// Объект сцены
// ---------------------------------------------------------------------
enum M3OBJECT_TYPE {

    M3_OBJECT_PLANE  = 1,
    M3_OBJECT_SQUARE,
    M3_OBJECT_TRIANGLE,
    M3_OBJECT_SPHERE,
};

struct m3object {

    int ai;             // Уникальный ID
    int id;             // Указатель на ID в объектах
    int type;           // 0-none, 1-plane, 2-sphere
};
