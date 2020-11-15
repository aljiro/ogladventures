#version 430

uniform float x;
uniform float y;

void main( void ){	
    gl_Position = vec4(x, y, 0.0, 1.0);
}
