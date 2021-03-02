#version 130

uniform sampler2D tex;
uniform float     time;
uniform vec3      cam;

// Вычисление рейкаста
float raycast(vec3 o, vec3 v, float r) {

    float a =     v.x*v.x + v.y*v.y + v.z*v.z;           // t*t
    float b = -2*(o.x*v.x + o.y*v.y + o.z*v.z);          // t
    float c =    (o.x*o.x + o.y*o.y + o.z*o.z) - r*r;    // 1
    float det = b*b - 4*a*c;

    if (det < 0) {
        return 0.0;
    } else {
        det = sqrt(det);
    }

    float k1 = (-b + det) / (2*a);
    float k2 = (-b - det) / (2*a);

    return abs(k1) < abs(k2) ? k1 : k2;
}

void main() {

    mat3 mx = mat3( 1.0, 0.0, 0.0, 0.0, cos(cam.x), sin(cam.x), 0.0, -sin(cam.x), cos(cam.x));
    mat3 my = mat3( cos(cam.y), 0.0, sin(cam.y), 0.0, 1.0, 0.0, -sin(cam.y), 0.0, cos(cam.y));
    mat3 mz = mat3( cos(cam.z), sin(cam.z), 0.0, -sin(cam.z), cos(cam.z), 0.0, 0.0, 0.0, 1.0);

    // Точка просмотра
    vec3  vp = mz * my * mx * vec3(gl_TexCoord[0].x - 0.5, gl_TexCoord[0].y - 0.5, 1.0);

    // Первоначальная высота 6,3m
    vec3  sp = vec3(0.0, 0.0, 1.000001 + time / 6380000.0);

    // Вычисление райкаста
    float di = raycast(sp, vp, 1.0 );

    if (di > 0) {

        // Вектор
        vec3  ls = vec3(1.0, -1.0, -2.0);
              ls = ls / length(ls);

        // Вектор
        vec3  am = vec3(1.0, -1.0, -2.0);
              am = am / length(am);

        vec3 coord  = vp * di;      // Расчет нормалей и координат
        vec3 sphere = coord - sp;
        vec3 norm   = sphere / length(sphere);
        vec3 dir    = -vp / length(vp);

        vec3  RL   = (2.0*norm*dot(norm, ls) - ls);
        float spec = dot(RL / length(RL), ls);
        float amli = dot(norm, am);

        if (amli < 0.0) amli = 0.0;
        spec = (spec > 0.0 ? pow(spec, 30.0) : 0.0) * 1.0;

        // Координата на точке
        float u = (0.5 * atan(norm.z, norm.x) / 3.141592) + 0.555;
        float v = 0.75 + (1.0 * asin(norm.y) / 3.141592);

        // Сложение цветов
        vec4 color = amli * texture2D(tex, vec2(u, v) ) + vec4(spec, spec, spec, 1.0);

        gl_FragColor = color;

    } else {
        discard;
    }
}
