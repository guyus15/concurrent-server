#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;

out vec3 colour;

void main()
{
    colour = aColour;
    
    gl_Position = vec4(aPos, 1.0f);
}