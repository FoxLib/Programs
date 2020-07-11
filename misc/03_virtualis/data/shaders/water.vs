#version 130 

attribute vec4 position;
attribute vec3 normal;
attribute vec2 texCoord;

// Принять данные о местоположении игрока
uniform     vec3    player;
uniform     float   timer;

// Передать фрагментному шейдеру
out         vec3    fs_player;
out         float   fs_timer;

// Передать во фрагментный шейдер
varying     vec4    pixel_coord;

void main() {

    fs_player       = player;
    fs_timer        = timer;
	gl_TexCoord[0]  = vec4(texCoord, 0.0, 0.0);
	gl_Position     = gl_ProjectionMatrix * gl_ModelViewMatrix * position; 
    
    // Передать истинную позицию в пространстве
    pixel_coord     = position;
}
