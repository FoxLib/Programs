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

    vec3 get_sphere_point(float, float, float);
    vec2 proj2d(vec3 p);
    vec3 intersect_sphere(vec3, float, vec3, vec3);

    vec3 mul(vec3, float);
    vec3 add(vec3, vec3);
    vec3 sub(vec3, vec3);
    vec3 norm(vec3);
    
    float dot(vec3 a, vec3 b);
};