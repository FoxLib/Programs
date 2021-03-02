using namespace std;

#include <vector>
#include <raycast.h>
#include <object.h>

class scene {

protected:

    // Определяется список объектов на сцене
    vector<object*> objs;

    // Найденная точка пересечения -1 если не пересеклость, 0..n-1
    int  intersect_id;
    vec3 intersect_point;
    vec3 intersect_normal;

    // Обработчик бросания лучей
    raycast* ray;

public:

    scene(int, int);

    int add(object*);

    // Поиск пересечения из точки `center` по вектору `dir`
    int intersect(vec3, vec3);

    // Getter
    vec3 get_normal();
};