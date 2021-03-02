#include <scene.h>
#include <stdio.h>

// Конструктор сцены создает raycast
scene::scene(int w, int h) {
    ray = new raycast(w, h);
}

// Добавление нового объекта на сцену [0..n-1]
int scene::add(object* obj) {

    objs.push_back(obj);
    return objs.size() - 1;
}

// Поиск точки пересечения
// @return 1 то найдено, 0 если не найдена
int scene::intersect(vec3 camera, vec3 dir) {

    unsigned int id;        
    vec3  cast_sphere;
    vec3  sphere_center;
    float cast_maximum = 100000.0;
    float nearest;

    intersect_id = -1;
    for (id = 0; id < objs.size(); id++) {

        switch (objs[id]->get_type()) {

            // Сфера
            case OBJ_SPHERE: 

                // Центр сферы
                sphere_center = objs[id]->get_center();

                // Вычисление точки пересечения
                cast_sphere  = ray->intersect_sphere(sphere_center, objs[id]->get_radius(), camera, dir);

                // Ближашая точка для нас
                nearest = cast_sphere.x < cast_sphere.y ? cast_sphere.x : cast_sphere.y;

                // Если точка пересечения и она впереди нас
                if (cast_sphere.z > 0 && nearest > 0 && cast_maximum > nearest) {

                    // Новая ближняя точка
                    cast_maximum = nearest; 

                    // Точка пересечения
                    intersect_point = 
                    {
                        camera.x + nearest*dir.x, 
                        camera.y + nearest*dir.y,
                        camera.z + nearest*dir.z
                    };

                    // Нормаль к сфере
                    intersect_normal = ray->normalize(
                    {
                        intersect_point.x - sphere_center.x, 
                        intersect_point.y - sphere_center.y,
                        intersect_point.z - sphere_center.z
                    });

                    intersect_id = id;
                }

                break;
        }
    }

    // Решение о пересечении
    return (intersect_id >= 0) ? 1 : 0;
}

vec3 scene::get_normal() {
    return intersect_normal;
}