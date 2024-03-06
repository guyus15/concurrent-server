#include "projectile.h"

#include <ranges>

#include "server.h"
#include "server_packet_dispatcher.h"

#include "physics/collision.h"

#include <common/world.h>

#include <glm/geometric.hpp>
#include <glm/detail/func_trigonometric.inl>

constexpr float PROJECTILE_SPEED = 100.0f,
                PROJECTILE_COLLISION_DISTANCE = 5.0f;
constexpr double EXPIRATION_TIME = 3;

static float GetRotationFromVector(glm::vec2 direction);
static glm::vec2 RotateVector(glm::vec2 vector, float rotation);

Projectile::Projectile(const glm::vec2 position, const glm::vec2 direction)
    : m_has_expired{ false },
      m_position{ position },
      m_velocity{ direction * PROJECTILE_SPEED },
      m_rotation{ 0.0f }
{
}

void Projectile::Update(const double dt)
{
    if (m_has_expired)
        return;

    // Update the projectile's position based on its velocity.
    m_position += m_velocity * static_cast<float>(dt);

    // Apply gravity
    m_velocity.y -= GRAVITY * GRAVITY_SCALE * static_cast<float>(dt);

    m_rotation = GetRotationFromVector(glm::normalize(m_velocity));

    ProjectileUpdate(*this);

    // Mark the projectile as dead so it will be removed.
    if (m_birth_clock.HasTimeElapsed(EXPIRATION_TIME))
        m_has_expired = true;

    for (const auto& [_, player] : Server::GetClientInfoMap() | std::views::values)
    {
        const glm::vec2 player_pos = player.GetPosition();
        if (Collision::ByDistance(player_pos, m_position, PROJECTILE_COLLISION_DISTANCE))
        {
            // Handle collision.
        }
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

glm::vec2 RotateVector(const glm::vec2 vector, const float rotation)
{
    return
    {
        cos(rotation) * vector.x - sin(rotation) * vector.y,
        sin(rotation) * vector.x + cos(rotation) * vector.y
    };
}
