#version 130 

uniform sampler2D tex;

in      vec3    fs_player;          // Положение игрока в {X,Y,Z}
in      float   fs_timer;           // Таймер
varying vec4    pixel_coord;        // Положение пикселя в {X,Y,Z}

void main()
{
    vec3 view_position  = fs_player.xyz;
    vec3 light_source   = vec3(0.0, 1.0, 1.0);
    vec3 coord          = pixel_coord.xyz;
    float ts            = 0.000025*fs_timer;
    
    // Координаты таймера
    vec2 t1_coord       = vec2(gl_TexCoord[0].s + cos(ts)*sin(ts), gl_TexCoord[0].t + 0.5*cos(ts));    
    vec2 t2_coord       = vec2(gl_TexCoord[0].s +         sin(ts), gl_TexCoord[0].t - 0.5*sin(ts));    
    
    vec3 tex1           = texture2D(tex, 0.75 * (gl_TexCoord[0].st + t1_coord)).xzy;
    vec3 tex2           = texture2D(tex, 1.25 * (gl_TexCoord[0].st + t2_coord)).xzy;
    
    // Вычисляем точку нормали в текстуры
    vec3 normal = 2.0 * mix(tex1, tex2, 0.5) - vec3(1.0);
         normal = normalize(normal + vec3(0.0, 0.125, 0.0));

    // Солнце направляется в данную точку текстуры
    vec3 lpnorm  = normalize(light_source);
    
    // Вектор направления вида игрок
    vec3 vpnorm  = normalize(view_position - coord);
                               
    // Вычисления вектора отражения
    vec3 ref_vec = reflect(-lpnorm, normal);
    
    // Уровень освещенности
    float light = max( dot(ref_vec, vpnorm), 0.25 );
    
    // аппроксимация Шлика a^b = a / (b – a*b + a) для a от нуля до единицы
    light /= 24.0 - light * 24.0 + light;

    gl_FragColor = vec4(light * vec3(1, 0.7, 0.5) + vec3(0, 0.3, 0.5), 0.8); 
    
}
