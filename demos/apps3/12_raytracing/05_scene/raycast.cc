#include <math.h>
#include <raycast.h>

raycast::raycast() {

    width = 0;
    height = 0;
    fov = 0;
}

// Конструктор
raycast::raycast(int w, int h) {

    width  = w;
    height = h;
    fov    = w / 2;
}

// Получение точки сферы по параметрам
vec3 raycast::get_sphere_point(float alpha, float beta, float radius) {

    vec3 pn;

    pn.x = radius * cos(alpha) * cos(beta);
    pn.y = radius * sin(beta);
    pn.z = radius * sin(alpha) * cos(beta);

    return pn;
}

// Проекция на экран
vec2 raycast::proj2d(vec3 p) {

    vec2 t;
    t.x = width/2  + p.x * fov / p.z;
    t.y = height/2 - p.y * fov / p.z;
    return t;
}

// center - Центр окружности
// radius - Радиус
// camera - Точка испускания луча
// dir     - Вектор луча
// Возврат x,y,z; x,y - это параметры t1, t2, и z>0 точки пересечения найдены
vec3 raycast::intersect_sphere(vec3 center, float radius, vec3 camera, vec3 dir) {

    vec3 result = {0, 0, -1};

    vec3 d = {camera.x - center.x,  // dx
              camera.y - center.y,  // dy
              camera.z - center.z}; // dz

    double a =   (dir.x*dir.x + dir.y*dir.y + dir.z*dir.z);
    double b = 2*(d.x*dir.x   + d.y*dir.y   + d.z*dir.z);
    double c =   (d.x*d.x     + d.y*d.y     + d.z*d.z - radius*radius);
    double D = b*b - 4*a*c;

    if (D >= 0) {

        result.x = (-b - sqrt(D)) / (2*a);
        result.y = (-b + sqrt(D)) / (2*a);
        result.z = 1;
    }

    return result;
}

// Скалярное произведение
float raycast::dot(vec3 a, vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

// Нормализация вектора
vec3 raycast::normalize(vec3 a) {

    float l = sqrt(dot(a, a));
    if (l == 0) l = 1;
    return {a.x / l, a.y / l, a.z / l};
}