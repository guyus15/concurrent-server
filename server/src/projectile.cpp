#include "projectile.h"
#include "server_packet_dispatcher.h"

#include <common/world.h>

#include <glm/geometric.hpp>

constexpr float PROJECTILE_SPEED = 10.0f;
constexpr double EXPIRATION_TIME = 3;

static float GetRotationFromVector(glm::vec2 direction);

Projectile::Projectile(const glm::vec2 position, const glm::vec2 direction)
    : m_has_expired{ false },
      m_position{ position },
      m_velocity{ direction * PROJECTILE_SPEED },
      m_rotation{ 0.0f }
{
}

void Projectile::Update(const double dt)
{
    if (!m_has_expired)
    {
        m_position += m_velocity;

        // Apply gravity
        m_velocity.y -= GRAVITY * static_cast<float>(dt);

        m_rotation = GetRotationFromVector(glm::normalize(m_velocity));

        ProjectileUpdate(*this);

        // Mark the projectile as dead so it will be removed.
        if (m_birth_clock.HasTimeElapsed(EXPIRATION_TIME))
            m_has_expired = true;
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

bool Projectile::HasExpired() const
{
    return m_has_expired;
}

float GetRotationFromVector(const glm::vec2 direction)
{
    return atan2(direction.y, direction.x);
}
