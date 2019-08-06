#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 vertex;

out vec3 particleColor;

uniform mat4 MVP;
uniform vec2 offset;
uniform vec4 color;

void main(){

	float scale = 10.0f;
    gl_Position = MVP*vec4(vertex.xyz*scale + offset), 1.0);
    particleColor = color;
}
