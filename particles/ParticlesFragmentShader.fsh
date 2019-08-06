#version 330 core

// Output data
out vec4 color;
in vec4 particleColor;

uniform sampler2D sprite;

void main()
{
    color = particleColor;
}
