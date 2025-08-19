#version 330 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D atlas;

void main()
{
    vec4 color = texture(atlas, uv);
    if (color.a < 0.5) discard;
    FragColor = color;
}