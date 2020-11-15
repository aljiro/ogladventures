#version 430

// Input data
in vec4 varyingColor;

// Output data
out vec4 color;

uniform mat4 MV;
uniform mat4 P;

void main( void )
{
    color = varyingColor;
}
