struct vec3 {
    float x, y, z;
};

struct vec2 {
    float x, y;
};

class raycast {

private:

    int   width, height;
    float fov;

public:

    raycast(int, int);

    struct vec3 get_sphere_point(float, float, float);
    struct vec2 proj2d(vec3 p);
    struct vec3 intersect_sphere(vec3, float, vec3, vec3);
};