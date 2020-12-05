#version 330 core
out vec4 FragColor;

uniform vec4 color;

uniform vec4 dimensions;

void main()
{
    vec2 pos = gl_FragCoord.xy;

    float x = dimensions.x;
    float y = dimensions.y;

    float r = (pow(pos.x - x, 2) / pow(dimensions.z / 2, 2)) + (pow(pos.y - y, 2) / pow(dimensions.w / 2, 2));

    if (r > 1)
    {
        discard;
    }
    else if (r > 0.95)
    {
        FragColor = vec4(color.rgb, (0.5 / r) * color.a);
    }
    else
    {
        FragColor = color;
    }
}
