#pragma once

#include "shader.h"
#include "texture2d.h"

#include <glm/vec2.hpp>

#include <array>

struct Transform
{
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;

    Transform();
};

struct Vertex
{
    glm::vec2 position;
    glm::vec2 texture_coordinate;
};

/**
 * \brief Represents a textured 2D mesh.
 */
class Sprite
{
public:
    Sprite(const Transform& transform, Texture2d texture);
    ~Sprite();

    void Initialise();
    void Dispose() const;

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
    void Render(const Shader& shader) const;

private:
    Transform m_transform;
    Texture2d m_texture;
    std::array<Vertex, 4> m_vertices;
    std::array<unsigned int, 6> m_indices;
    GLuint m_vao, m_vbo, m_ebo;
};
