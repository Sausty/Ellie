#version 450 core

out vec4 FragColor;

in vec2 fTexCoords;

uniform sampler2D Texture;

void main()
{
    FragColor = texture(Texture, fTexCoords);
}