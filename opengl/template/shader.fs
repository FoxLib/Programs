#version 120
uniform sampler2D tex;
void main() {

    gl_FragColor = texture2D(tex, gl_TexCoord[0].st );
    //gl_FragColor = vec4(gl_TexCoord[0].st, 0, 1) + texture2D(tex, gl_TexCoord[0].st );
}
