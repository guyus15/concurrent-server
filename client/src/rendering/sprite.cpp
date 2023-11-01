#include "sprite.h"

Sprite::Sprite(const Transform& position, Texture2d texture)
    : m_transform{position},
      m_texture{std::move(texture)}
{
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

void Sprite::Render()
{
    // Implement drawing code here.
}
