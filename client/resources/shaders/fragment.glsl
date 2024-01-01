#version 430 core

in vec2 texCoords;

out vec4 fragColour;

uniform sampler2D this_texture;

void main()
{
    fragColour = texture(this_texture, texCoords);
}