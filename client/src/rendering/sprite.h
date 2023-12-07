#pragma once

#include "texture2d.h"

#include <glm/vec2.hpp>

struct Transform
{
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;
};

/**
 * \brief Represents a textured 2D mesh.
 */
class Sprite
{
public:
    Sprite(const Transform& position, Texture2d texture);

    /**
     * \brief Set the position of the sprite.
     * \param value The new position value.
     */
    void SetPosition(const glm::vec2& value);

    /**
     * \brief Set the scale of the sprite.
     * \param value The new scale value.
     */
    void SetScale(const glm::vec2& value);

    /**
     * \brief Set the rotation of the sprite.
     * \param value The new rotation value.
     */
    void SetRotation(float value);

    /**
     * \brief Render the sprite to the screen at its current position.
     */
    void Render();

private:
    Transform m_transform;
    Texture2d m_texture;
};
