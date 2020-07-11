#include <vec.h>

enum ObjectType {

    OBJ_NULL,           // Нет объекта
    OBJ_PLANE,          // Плоскость
    OBJ_SPHERE          // Сфера
};

class object {

protected:

    int type;

    // Для сферы
    vec3  center;
    float radius;

    // Материал
    vec3  ambient_light;      // Фоновый свет
    float specularity;        // Отражательная способность 0..1
    float diffuse;            // Диффузное отражение 0..1

public:

    object(vec3, float);      // Создание сферы

    int   get_type();
    vec3  get_center();
    float get_radius();
};