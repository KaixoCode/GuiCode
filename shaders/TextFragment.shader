#version 330 core
in vec2 TexCoords;
out vec4 col;

uniform sampler2D text;
uniform vec4 color;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    vec4 c = color * sampled;
    if (c.w == 0)
        discard;
    else
        col = c;
}