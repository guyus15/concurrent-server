#include "projectile.h"

#include <ranges>

#include "server.h"
#include "server_packet_dispatcher.h"

#include "physics/collision.h"

#include <common/world.h>

#include <glm/geometric.hpp>
#include <glm/detail/func_trigonometric.inl>

constexpr float PROJECTILE_SPEED = 150.0f,
                PROJECTILE_COLLISION_DISTANCE = 5.0f;
constexpr double PROJECTILE_EXPIRATION_TIME = 3;
constexpr int PROJECTILE_DAMAGE = 10.0f;

static float GetRotationFromVector(glm::vec2 direction);
static glm::vec2 RotateVector(glm::vec2 vector, float rotation);

Projectile::Projectile(const glm::vec2 position, const glm::vec2 direction, const unsigned int src_id)
    : m_src_id{ src_id },
      m_has_expired{ false },
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
    if (m_birth_clock.HasTimeElapsed(PROJECTILE_EXPIRATION_TIME))
        m_has_expired = true;

    bool collision_occurred = false;

    for (auto& [_, player] : Server::GetClientInfoMap() | std::views::values)
    {
        // If the projectile has already been involved in a collision, don't check for further collisions.
        if (collision_occurred)
            return;

        // If the source of the projectile is the player that we're trying to
        // check for a collision with, skip.
        if (player.GetId() == m_src_id)
            continue;

        // If this player's health is already 0, skip.
        if (player.GetCurrentHealth() == 0)
            continue;

        const glm::vec2 player_scale = player.GetScale();
        const glm::vec2 player_pos = player.GetPosition();
        const glm::vec2 player_centre_pos = { player_pos.x + player_scale.x / 2, player_pos.y - player_scale.y / 2 };
        const glm::vec2 projectile_centre_pos = {
            m_position.x + PROJECTILE_SCALE.x / 2, m_position.y + PROJECTILE_SCALE.y / 2
        };

        if (Collision::ByDistance(player_centre_pos, projectile_centre_pos, PROJECTILE_COLLISION_DISTANCE))
        {
            // To stop multiple collisions from occurring, mark the projectile as such.
            collision_occurred = true;

            // A collision has occurred, apply damage to the relevant player.
            player.RemoveHealth(PROJECTILE_DAMAGE);
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
