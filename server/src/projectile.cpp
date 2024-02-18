#include "projectile.h"

constexpr float PROJECTILE_SPEED = 10.0f;

static float GetRotationFromVector(glm::vec2 direction);

Projectile::Projectile(const glm::vec2 position, const glm::vec2 direction, const unsigned int from_client)
    : m_position{ position },
      m_velocity{ direction * PROJECTILE_SPEED },
      m_from_client{ from_client }
{
    m_rotation = GetRotationFromVector(direction);
}

void Projectile::Update(double dt)
{
}

float GetRotationFromVector(glm::vec2 direction)
{
    return 0.0f;
}
