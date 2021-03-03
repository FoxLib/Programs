#version 120

uniform sampler2D tex;
uniform float utime;

#define OCTAVES 6
#define PI 3.141592

float atan2(float y, float x) {
    return (PI + atan(y, x)) / (2*PI);
}

float random (vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233)))*43758.5453123);
}

float noise (vec2 st) {

    vec2 i = floor(st);
    vec2 f = fract(st);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));
    vec2  u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) + (c-a)*u.y*(1.0-u.x) + (d-b)*u.x*u.y;
}

float fbm(vec2 st) {

    float value     = 0.0;
    float amplitude = .5;
    float frequency = 0.;

    for (int i = 0; i < OCTAVES; i++) {
        value += amplitude * noise(st);
        st *= 2.;
        amplitude *= .5;
    }

    return value;
}

void main() {

    vec4  t = texture2D(tex, gl_TexCoord[0].st );
    float m = fbm((gl_TexCoord[0].st)*8.0 + vec2(utime, 0.0));

    gl_FragColor = m < 0.5 ? vec4(0.0, 0.0, 1.5*m, 1.0) : vec4(m*0.5, m, m*0.5, 1.);
    // gl_FragColor = vec4(0.0, atan2(gl_TexCoord[0].y - 0.5, 0.5 - gl_TexCoord[0].x), 0.0, 1.0);
}
