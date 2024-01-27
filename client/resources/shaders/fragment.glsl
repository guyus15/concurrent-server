#version 430 core

in vec2 texCoords;

out vec4 fragColour;

uniform sampler2D this_texture;

void main()
{
    vec4 texColour = texture(this_texture, texCoords);
    // Discard any fragments with an alpha value below 0.1.
    if (texColour.a < 0.1)
        discard;
    fragColour = texColour;
}