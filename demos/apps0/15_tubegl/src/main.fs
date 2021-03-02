#version 120

const float PI = 3.141592653;

uniform sampler2D tex;
uniform float     time;
uniform vec3      cam;

void main() {

    // Viewport
    vec3 n = vec3(2.0*(gl_TexCoord[0].st - vec2(0.5, 0.5)), 1.0);

    // Матрица поворотов
    // ---------------------
    mat3 mx = mat3( 1.0, 0.0, 0.0, 0.0, cos(cam.x), sin(cam.x), 0.0, -sin(cam.x), cos(cam.x));
    mat3 my = mat3( cos(cam.y), 0.0, sin(cam.y), 0.0, 1.0, 0.0, -sin(cam.y), 0.0, cos(cam.y));
    mat3 mz = mat3( cos(cam.z), sin(cam.z), 0.0, -sin(cam.z), cos(cam.z), 0.0, 0.0, 0.0, 1.0);
    n = mz * my * mx * n;
    
    // Расчет (u, v) точки текстуры
    // ---------------------
    vec3  t = n / (sqrt( n.x*n.x + n.y*n.y ) * 1.0);
    float u = atan(t.x, t.y) / PI * 1.0;
    float v = t.z - time;

    // Платформа
    // ---------------------
    vec3  cl;
    float tm = 3.0 / n.z;
    vec2  A  = vec2(n.x, n.y) * tm;
    float rs = length(A);
    float alpha = 1.0;
    
    
    // Бордюры
    // ---------------------

    cl = texture2D(tex, vec2(u, v) ).xyz;
    t.z = abs(t.z); if (t.z < 1.0) t.z = 1.0;
    cl = cl / t.z;

    if (rs < 3.0 && rs > 2.0) {
        cl = texture2D(tex, 0.1*vec2(A.x, A.y) ).xyz;
    }
    else if (sin(v) > 0.999 || sin(u*20.0) > 0.95) {
    
        // ok
        
    } else {

        discard;        
    }
    
    gl_FragColor = vec4(cl, alpha );
}
