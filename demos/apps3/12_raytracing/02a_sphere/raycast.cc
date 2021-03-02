#include <math.h>
#include <raycast.h>

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

// Умножение на число
vec3 raycast::mul(vec3 a, float b) {
    return {a.x*b, a.y*b, a.z*b};
}

// Сложение векторов
vec3 raycast::add(vec3 a, vec3 b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

// Вычитание векторов
vec3 raycast::sub(vec3 a, vec3 b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

// Скалярное произвдение
float raycast::dot(vec3 a, vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

// Нормализация
vec3 raycast::norm(vec3 a) {
    float r = 1.0/sqrt(dot(a,a));
    return {a.x*r, a.y*r, a.z*r};
}
