#include "projectile.h"

constexpr float PROJECTILE_SPEED = 10.0f;

static float GetRotationFromVector(glm::vec2 direction);

Projectile::Projectile(const glm::vec2 position, const glm::vec2 direction)
    : m_position{ position },
      m_velocity{ direction * PROJECTILE_SPEED }
{
    m_rotation = GetRotationFromVector(direction);
}

void Projectile::Update(double dt)
{
}

UUID Projectile::GetId() const
{
    return m_id;
}

glm::vec2 Projectile::GetPosition() const
{
    return m_position;
}

float Projectile::GetRotation() const
{
    return m_rotation;
}

float GetRotationFromVector(glm::vec2 direction)
{
    return 0.0f;
}
