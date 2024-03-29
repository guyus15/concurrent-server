#include "projectile.h"

#include "server.h"
#include "server_packet_dispatcher.h"

#include "physics/collision.h"

#include <common/world.h>
#include <common/level_manager.h>

#include <glm/geometric.hpp>
#include <glm/detail/func_trigonometric.inl>

#include <ranges>

constexpr float PROJECTILE_SPEED = 150.0f,
                PROJECTILE_COLLISION_DISTANCE = 5.0f;
constexpr double PROJECTILE_EXPIRATION_TIME = 3;
constexpr int PROJECTILE_DAMAGE = 10;

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


    glm::vec2 rotated_scale_x = RotateVector({ PROJECTILE_SCALE.x / 2.0f, 0 }, m_rotation);
    glm::vec2 rotated_scale_y = RotateVector({ 0, PROJECTILE_SCALE.y / 2.0f }, m_rotation);

    const Collision::AABB projectile_aabb{
        { RotateVector({ -PROJECTILE_SCALE.x / 2.0f, PROJECTILE_SCALE.y / 2.0f }, m_rotation) + m_position },
        { RotateVector({ PROJECTILE_SCALE.x / 2.0f, PROJECTILE_SCALE.y / 2.0f }, m_rotation) + m_position },
        { RotateVector({ PROJECTILE_SCALE.x / 2.0f, -PROJECTILE_SCALE.y / 2.0f }, m_rotation) + m_position },
        { RotateVector({ -PROJECTILE_SCALE.x / 2.0f, -PROJECTILE_SCALE.y / 2.0f }, m_rotation) + m_position }
    };

    for (auto& [_, player] : Server::GetClientInfoMap() | std::views::values)
    {
        // If the source of the projectile is the player that we're trying to
        // check for a collision with, skip.
        if (player.GetId() == m_src_id)
            continue;

        // If this player's health is already 0, skip.
        if (player.GetCurrentHealth() == 0)
            continue;

        const glm::vec2 player_pos = player.GetPosition();
        const glm::vec2 player_scale = player.GetScale();

        const Collision::AABB player_aabb{
            { player_pos.x - player_scale.x / 2.0f, player_pos.y + player_scale.y / 2.0f },
            { player_pos.x + player_scale.x / 2.0f, player_pos.y + player_scale.y / 2.0f },
            { player_pos.x + player_scale.x / 2.0f, player_pos.y - player_scale.y / 2.0f },
            { player_pos.x - player_scale.x / 2.0f, player_pos.y - player_scale.y / 2.0f }
        };

        if (Collision::AABBtoAABB(projectile_aabb, player_aabb, nullptr))
        {
            // A collision has occurred, apply damage to the relevant player.
            player.RemoveHealth(PROJECTILE_DAMAGE);

            // To stop multiple collisions from occurring, mark the projectile as expired and break.
            m_has_expired = true;
            break;
        }
    }

    for (const auto& platform : LevelManager::GetActive().GetByType(LevelContent::Type::Platform))
    {
        const Collision::AABB platform_aabb{
            { platform.position.x - platform.scale.x / 2.0f, platform.position.y + platform.scale.y / 2.0f },
            { platform.position.x + platform.scale.x / 2.0f, platform.position.y + platform.scale.y / 2.0f },
            { platform.position.x + platform.scale.x / 2.0f, platform.position.y - platform.scale.y / 2.0f },
            { platform.position.x - platform.scale.x / 2.0f, platform.position.y - platform.scale.y / 2.0f }
        };

        if (Collision::AABBtoAABB(projectile_aabb, platform_aabb, nullptr))
        {
            m_has_expired = true;
            break;
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
        cos(-rotation) * vector.x - sin(-rotation) * vector.y,
        sin(-rotation) * vector.x + cos(-rotation) * vector.y
    };
}

bool operator==(const Projectile p1, const Projectile p2)
{
    return p1.GetId() == p2.GetId();
}

bool operator!=(const Projectile p1, const Projectile p2)
{
    return !(p1 == p2);
}
