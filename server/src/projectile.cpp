#include "projectile.h"
#include "server_packet_dispatcher.h"

#include <common/world.h>

constexpr float PROJECTILE_SPEED = 10.0f;
constexpr double EXPIRATION_TIME = 3;

static float GetRotationFromVector(glm::vec2 direction);

Projectile::Projectile(const glm::vec2 position, const glm::vec2 direction)
    : m_has_died{ false },
      m_position{ position },
      m_velocity{ direction * PROJECTILE_SPEED }
{
    m_rotation = GetRotationFromVector(direction);
}

void Projectile::Update(const double dt)
{
    if (!m_has_died)
    {
        m_position += m_velocity;

        // Apply gravity
        m_velocity.y -= GRAVITY * static_cast<float>(dt);

        ProjectileUpdate(*this);

        // Mark the projectile as dead so it will be removed.
        if (m_birth_clock.HasTimeElapsed(EXPIRATION_TIME))
            m_has_died = true;
    }

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
