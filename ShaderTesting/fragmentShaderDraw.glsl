#version 330 core

out vec4 color;
in vec2 fragPos;

uniform sampler2D tex;

uniform float refractorStartPos = -0.3, refractorEndPos = -0.4;

void main()
{
    color = (abs(texture(tex, (fragPos+vec2(1,1))*0.5)));
//    float r =texture(tex, (fragPos+vec2(1,1))*0.5).r;
//    color = vec4(r,(fragPos.y < refractorStartPos && fragPos.y > refractorEndPos) ? 0.1 : 0,-r,1);
}