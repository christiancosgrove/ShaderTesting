#version 330 core

out vec4 color;
in vec2 fragPos;

uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
    color = texture(tex0, (fragPos+vec2(1,1))*0.5);
}