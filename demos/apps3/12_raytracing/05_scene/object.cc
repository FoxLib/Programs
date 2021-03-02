#include <object.h>

// Создание сферы
object::object(vec3 _center, float _radius) {

    type   = OBJ_SPHERE;
    center = _center;
    radius = _radius;

    ambient_light = {0.0, 0.0, 0.0};
    specularity   = 1.0;
    diffuse       = 1.0;
}

// Возвратить тип объекта
int object::get_type() {
    return type;
}

vec3 object::get_center() {
    return center;
}

float object::get_radius() {
    return radius;
}