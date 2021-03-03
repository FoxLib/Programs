#version 120

uniform sampler2D tex;
uniform float utime;

#define OCTAVES 6

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
    float m = fbm((gl_TexCoord[0].st)*8.0 + vec2(utime, 0.0) + t.st);

    gl_FragColor = vec4(m, m, m, 1);
}
