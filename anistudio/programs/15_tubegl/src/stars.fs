#version 120

const float PI = 3.141592653;

uniform sampler2D tex;
uniform float     time;
uniform vec3      cam;

void main() {

    // Viewport
    vec3  n = vec3(2.0*(gl_TexCoord[0].st - vec2(0.5, 0.5)), 1.0);

    float u = n.y - cam.x * 3.00;
    float v = n.x + cam.z * 3.00;
    float a = 1.0;

    if (time < 10000.0) a = time/10000.0;
    
    vec3 cl = texture2D(tex, vec2(u, v) ).xyz;
    gl_FragColor = vec4(cl, a);
}
