#version 130 

uniform sampler2D tex;

in      vec3    FragNormal;
varying vec4    pixel_coord;        // Положение пикселя в {X,Y,Z}
     
void main()
{
    vec2 coordt  = vec2(gl_TexCoord[0].s, gl_TexCoord[0].t);
    vec4 color   = texture2D(tex, coordt);
    
    // Расчет
    float light_i = dot( FragNormal, normalize(vec3(0.75, 1.0, 0.0)) );
    if (light_i < 0.0) light_i = 0.0;

    vec3 t_color = mix(color.rgb * light_i, vec3(1,1,1), 0.0);

    // Применение интенсивности
    if (pixel_coord.y < 0)
        t_color = mix(t_color, vec3(0.0, 0.2, 0.3), 0.5);
        
    gl_FragColor = vec4(t_color, 1.0);
}

