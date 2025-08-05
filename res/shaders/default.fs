#version 330 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D atlas;

void main()
{
    FragColor = texture(atlas, uv);
}