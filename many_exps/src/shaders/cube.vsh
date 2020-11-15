#version 430

layout (location=0) in vec3 position;

uniform mat4 MV;
uniform mat4 P;

out vec4 varyingColor;

void main( void ){	
    gl_Position = P*MV*vec4(position, 1.0);
    varyingColor = vec4(position, 1.0)*0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}
