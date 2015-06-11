#version 330 core

layout(location = 0) in vec3 position;

uniform float time;
out vec2 fragPos;

void main()
{
    gl_Position = vec4(position,1.0);
    fragPos = position.xy;
}