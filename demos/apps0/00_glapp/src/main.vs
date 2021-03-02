#version 130

attribute vec4 position;

void main() {

    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position    = gl_ProjectionMatrix * gl_ModelViewMatrix * position;
}

