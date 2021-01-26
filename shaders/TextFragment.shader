#version 330 core
in vec2 texCoord;
out vec4 col;

uniform sampler2D theTexture;
uniform vec4 color;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(theTexture, texCoord).r);
    vec4 c = color * sampled;
    if (c.w == 0)
        discard;
    else
        col = c;
}