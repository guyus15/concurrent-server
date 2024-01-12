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
    Sprite();
    explicit Sprite(Texture2d texture);
    ~Sprite();

    Sprite(const Sprite& other) = default;
    Sprite& operator=(const Sprite& other) = default;

    Sprite(Sprite&& other) noexcept = default;
    Sprite& operator=(Sprite&& other) noexcept = default;

    /**
     * \brief Render the sprite to the screen at its current position.
     */
    void Draw(const Transform& transform, const Shader& shader) const;

private:
    Texture2d m_texture;
    std::array<Vertex, 4> m_vertices{};
    std::array<unsigned int, 6> m_indices{};
    GLuint m_vao, m_vbo, m_ebo;

    void Initialise();
    void Dispose() const;
};
