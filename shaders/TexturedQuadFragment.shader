#version 330 core
layout(location = 0) out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D theTexture;

void main()
{
    FragColor = texture(theTexture, texCoord).rgba;
}
