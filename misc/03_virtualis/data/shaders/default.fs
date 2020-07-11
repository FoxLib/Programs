#version 130 

uniform sampler2D tex;

void main()
{
    vec2 coordt  = vec2(gl_TexCoord[0].s, gl_TexCoord[0].t);
    vec4 color   = texture2D(tex, coordt);
    gl_FragColor = color;  
}
