#version 330 core


const float PI = 3.14159;

const int n = 10;
out vec4 color;

in vec2 fragPos;

uniform float time;

uniform sampler2D tex;
uniform sampler2D texPrev;

const float c2=0.01f;
const float dx = 1/720.0f;
const float dy = 1/720.0f;

uniform float refractorStartPos = -0.3, refractorEndPos = -0.4;

void main()
{
//    vec2 polar = vec2(sqrt(fragPos.x*fragPos.x+fragPos.y*fragPos.y), atan(fragPos.y/fragPos.x)+time/100);
    
//    color = vec3(sin(time/100) ,cos(time/10),0.5)*sin(time/10.* PI*polar.x)*sin(time/10.*PI*polar.y) + vec3(0.5,0.5,0.5);
//    color = texture(tex,fragPos).xyz;
    const float dt = 0.1;
    vec2 fnew =(fragPos+vec2(1,1))*0.5;
    const float coupling = 0.0;
    
    
    float c2new =c2+0.2*(fragPos.y*fragPos.y+fragPos.x*fragPos.x);
//    float c2new = ((fragPos.y > refractorStartPos || fragPos.y < refractorEndPos) ? c2 : 0.15*c2);
    
    color = (c2new*(texture(tex,fnew+vec2(dx,0)) + texture(tex,fnew+vec2(-dx,0)) + texture(tex,fnew+vec2(0,dy)) + texture(tex,fnew+vec2(0,-dy)) - 4*texture(tex,fnew)) + 2*texture(tex,fnew)-texture(texPrev,fnew));
}