#include "sprite.h"

#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

Sprite::Sprite()
    : m_vao{},
      m_vbo{},
      m_ebo{}
{
}

Sprite::Sprite(Texture2d texture)
    : m_texture{ std::move(texture) },
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

Sprite::Sprite(Sprite&& other) noexcept
{
    m_texture = other.m_texture;
    m_vertices = other.m_vertices;
    m_indices = other.m_indices;
    m_vao = other.m_vao;
    m_vbo = other.m_vbo;
    m_ebo = other.m_ebo;
}

Sprite& Sprite::operator=(Sprite&& other) noexcept
{
    if (this == &other)
        return *this;

    m_texture = other.m_texture;
    m_vertices = other.m_vertices;
    m_indices = other.m_indices;
    m_vao = other.m_vao;
    m_vbo = other.m_vbo;
    m_ebo = other.m_ebo;

    return *this;
}

void Sprite::Draw(const Transform& transform, const Shader& shader) const
{
    shader.Use();

    glm::mat4x4 model{ 1.0f };

    model = glm::translate(model, glm::vec3{ transform.position.x, transform.position.y, 0.0f });
    model = glm::rotate(model, transform.rotation, glm::vec3{ 0.0f, 0.0f, 1.0f });
    model = glm::scale(model, glm::vec3{ -transform.scale.x, transform.scale.y, 1.0f });

    shader.SetMat4x4("model", model);

    m_texture.Bind();

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}
