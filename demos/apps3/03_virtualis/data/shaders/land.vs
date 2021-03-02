#version 130 

attribute vec4 position;
attribute vec3 normal;
attribute vec2 texCoord;

out     vec3    FragNormal;
varying vec4    pixel_coord;

void main() {

    FragNormal      = normal;
    pixel_coord     = position;
	gl_TexCoord[0]  = vec4(texCoord, 0.0, 0.0);
	gl_Position     = gl_ProjectionMatrix * gl_ModelViewMatrix * position;
}
