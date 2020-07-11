#include <vec.h>

class raycast {

private:

    int   width, height;
    float fov;

public:

    raycast();
    raycast(int, int);

    vec3 get_sphere_point(float, float, float);
    vec2 proj2d(vec3 p);
    vec3 intersect_sphere(vec3, float, vec3, vec3);

    float dot(vec3, vec3);
    vec3 normalize(vec3);
};