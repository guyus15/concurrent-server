#pragma once

#include "shader.h"
#include "texture2d.h"
#include "transform.h"

#include <glm/vec2.hpp>

#include <array>

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

    Sprite(const Sprite&) = default;
    Sprite& operator=(const Sprite&) = default;

    Sprite(Sprite&&) noexcept = default;
    Sprite& operator=(Sprite&&) noexcept = default;

    void Initialise();
    void Dispose() const;

    /**
     * \brief Render the sprite to the screen at its current position.
     */
    void Draw(const Shader& shader) const;

private:
    Texture2d m_texture;
    std::array<Vertex, 4> m_vertices{};
    std::array<unsigned int, 6> m_indices{};
    GLuint m_vao, m_vbo, m_ebo;
};
