#version 430 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;   

uniform mat4 model;
uniform mat4 projection;

out vec2 texCoords;

void main()
{
    texCoords = aTexCoords;
    
    gl_Position = projection * model * vec4(aPos, 0.0f, 1.0f);
}