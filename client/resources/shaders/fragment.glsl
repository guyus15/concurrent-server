#version 460 core

in vec3 colour;
in vec2 texCoords;

out vec4 fragColour;

uniform sampler2D this_texture;
uniform bool use_texture;

void main()
{
    if (use_texture)
        fragColour = texture(this_texture, texCoords);
    else
        fragColour = vec4(colour, 1.0f);
}