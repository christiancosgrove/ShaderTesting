#version 330 core


const float PI = 3.14159;

const int n = 10;
out vec4 color;

in vec2 fragPos;

uniform float time;

uniform sampler2D tex;
uniform sampler2D texPrev;
const float ru = 0.000;
const float rv = 0.000;
const float f = 0.0001;
const float k = 0.0001;
const float dx = 1/720.0f;
const float dy = 1/720.0f;


void main()
{
    
    vec2 fnew =(fragPos+vec2(1,1))*0.5;
    
    float u = texture(tex, fnew).r;
    float v = texture(tex, fnew).g;
    
    
    color.r = ru*(texture(tex,fnew+vec2(dx,0)).r + texture(tex,fnew+vec2(-dx,0)).r + texture(tex,fnew+vec2(0,dy)).r + texture(tex,fnew+vec2(0,-dy)).r - 4*texture(tex,fnew).r) - u*v*v + f*(1-u) + texture(tex, fnew).r;
    color.g = rv*(texture(tex,fnew+vec2(dx,0)).g + texture(tex,fnew+vec2(-dx,0)).g + texture(tex,fnew+vec2(0,dy)).g + texture(tex,fnew+vec2(0,-dy)).g - 4*texture(tex,fnew).g) + u*v*v - (f+k)*v + texture(tex, fnew).g;
    color.b=0;
}