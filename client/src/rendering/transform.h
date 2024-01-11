#pragma once

#include <glm/vec2.hpp>

struct Transform
{
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;

    Transform()
        : position{ 0.0f, 0.0f },
          scale{ 1.0f, 1.0f },
          rotation{ 0.0f }
    {
    }
};
