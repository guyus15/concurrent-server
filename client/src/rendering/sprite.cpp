#include "sprite.h"

#include <common/graphics/screen.h>

#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

Transform::Transform()
    : position{ 0.0f, 0.0f },
      scale{ 1.0f, 1.0f },
      rotation{ 0.0f }
{
}

Sprite::Sprite(const Transform& transform, Texture2d texture)
    : m_transform{ transform },
      m_texture{ std::move(texture) },
      m_vao{},
      m_vbo{},
      m_ebo{}
{
    Initialise();
}

Sprite::~Sprite()
{
    Dispose();
}

void Sprite::Initialise()
{
    const auto [fst, snd] = GetCurrentResolution(glfwGetPrimaryMonitor());

    // The width is the height multiplied by the aspect ratio, so we can work out
    // the vertices that the sprite should have, assuming the height is 1, using the
    // texture's aspect ratio.
    const float texture_aspect_ratio = m_texture.GetAspectRatio();

    m_vertices[0].position = { texture_aspect_ratio / 2.0f, 0.5f };
    m_vertices[1].position = { -texture_aspect_ratio / 2.0f, 0.5f };
    m_vertices[2].position = { texture_aspect_ratio / 2.0f, -0.5f };
    m_vertices[3].position = { -texture_aspect_ratio / 2.0f, -0.5f };

    m_vertices[0].texture_coordinate = { 0.0f, 1.0f };
    m_vertices[1].texture_coordinate = { 1.0f, 1.0f };
    m_vertices[2].texture_coordinate = { 0.0f, 0.0f };
    m_vertices[3].texture_coordinate = { 1.0f, 0.0f };

    // Assign the relevant vertices as indices to construct a quadrilateral shape.
    m_indices = { 0, 1, 3, 0, 2, 3 };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, position)));

    // Vertex texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, texture_coordinate)));

    glBindVertexArray(0);
}

void Sprite::Dispose() const
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void Sprite::SetPosition(const glm::vec2& value)
{
    m_transform.position = value;
}

void Sprite::SetScale(const glm::vec2& value)
{
    m_transform.scale = value;
}

void Sprite::SetRotation(const float value)
{
    m_transform.rotation = value;
}

void Sprite::Render(const Shader& shader) const
{
    shader.Use();

    glm::mat4x4 model{ 1.0f };

    model = glm::translate(model, glm::vec3{ m_transform.position.x, m_transform.position.y, 0.0f });
    model = glm::rotate(model, glm::radians(m_transform.rotation), glm::vec3{ 0.0f, 0.0f, 1.0f });
    model = glm::scale(model, glm::vec3{ -m_transform.scale.x, m_transform.scale.y, 1.0f });

    shader.SetMat4x4("model", model);

    m_texture.Bind();

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}
