#include "player.h"
#include "game.h"
#include "server_packet_dispatcher.h"

#include "physics/collision.h"

#include <common/level_manager.h>
#include <common/world.h>

#include <common/utils/logging.h>

constexpr glm::vec2 PLAYER_START_POSITION = { 0.0f, 0.0f },
                    PLAYER_SCALE{ 10.0f, 10.0f };
constexpr float PLAYER_MOVEMENT_SPEED = 50.0f,
                PLAYER_JUMP_SPEED = 75.0f,
                PLAYER_DAMPENING_FACTOR = 0.1f,
                PLAYER_SNAP_TO_GROUND_DISTANCE = 0.001f;
constexpr double PLAYER_FIRE_RATE = 0.1;
constexpr int PLAYER_MAX_HEALTH = 100;

Player::Player()
    : m_id{ 0 },
      m_position{ PLAYER_START_POSITION },
      m_velocity{ 0.0f, 0.0f },
      m_scale{ PLAYER_SCALE },
      m_weapon_rotation{ 0 },
      m_on_platform{},
      m_health{ PLAYER_MAX_HEALTH }

{
}

void Player::Update(const double dt)
{
    // Acquire lock on player guard.
    std::unique_lock guard_lock{ m_guard };

    // Update the player's position based on their velocity.
    m_position += m_velocity * static_cast<float>(dt);

    // Apply gravity
    m_velocity.y -= GRAVITY * GRAVITY_SCALE * static_cast<float>(dt);

    if (m_position.y <= GROUND_HEIGHT + PLAYER_SCALE.y + PLAYER_SNAP_TO_GROUND_DISTANCE)
    {
        m_velocity.y = 0;
        m_position.y = GROUND_HEIGHT + PLAYER_SCALE.y;
    }

    // Gradually bring the x velocity to zero.
    m_velocity = { m_velocity.x * PLAYER_DAMPENING_FACTOR, m_velocity.y };

    HandleCollisions();
}

void Player::AddHealth(const int amount)
{
    if ((m_health += amount) > PLAYER_MAX_HEALTH)
        m_health = PLAYER_MAX_HEALTH;

    PlayerHealthUpdate(m_id, *this);
}

void Player::RemoveHealth(const int amount)
{
    if ((m_health -= amount) < 0)
        m_health = 0;

    PlayerHealthUpdate(m_id, *this);
}

int Player::GetCurrentHealth() const
{
    return m_health;
}

void Player::ProcessInput(const bool key_pressed_down_w, const bool key_pressed_a, const bool key_pressed_d,
                          const bool left_mouse_btn_pressed)
{
    // Acquire lock on player guard.
    std::unique_lock guard_lock{ m_guard };

    if (key_pressed_down_w && IsGrounded())
    {
        m_velocity.y = PLAYER_JUMP_SPEED;
        m_on_platform = false;
    }

    if (key_pressed_a)
        m_velocity.x = -PLAYER_MOVEMENT_SPEED;
    if (key_pressed_d)
        m_velocity.x = PLAYER_MOVEMENT_SPEED;

    if (left_mouse_btn_pressed && m_weapon_clock.HasTimeElapsed(PLAYER_FIRE_RATE))
    {
        // Convert the weapon's rotation to a direction.
        const glm::vec2 weapon_direction = { cos(m_weapon_rotation), sin(m_weapon_rotation) };
        Game::SpawnProjectile(m_position, weapon_direction, m_id);
    }
}

void Player::HandleCollisions()
{
    Level& current_level = LevelManager::GetActive();

    const Collision::AABB player_aabb{
        m_position,
        { m_position.x + m_scale.x, m_position.y },
        { m_position.x + m_scale.x, m_position.y + m_scale.y },
        { m_position.x, m_position.y + m_scale.y }
    };

    for (const auto& platform : current_level.GetByType(LevelContent::Type::Platform))
    {
        const Collision::AABB platform_aabb{
            platform.position,
            { platform.position.x + platform.scale.x, platform.position.y },
            { platform.position.x + platform.scale.x, platform.position.y + platform.scale.y },
            { platform.position.x, platform.position.y + platform.scale.y }
        };

        if (Collision::AABBtoAABB(player_aabb, platform_aabb))
        {
            if (m_position.y + m_scale.y > platform.position.y && m_position.y < platform.position.y)
            {
                // Collision happened from the top of the player.
                m_position.y = platform.position.y - m_scale.y;
                m_velocity.y = 0;
            }
            else if (m_position.y < platform.position.y + platform.scale.y && m_position.y + m_scale.y > platform.
                position.y + platform.scale.y)
            {
                // Collision happened from the bottom of the player.
                m_position.y = platform.position.y + platform.scale.y;
                m_velocity.y = 0;
            }
            else if (m_position.x + m_scale.x > platform.position.x && m_position.x < platform.position.x)
            {
                // Collision happened from the left side of the player.
                m_position.x = platform.position.x - m_scale.x;
                m_velocity.x = 0;
            }
            else if (m_position.x < platform.position.x + platform.scale.x && m_position.x + m_scale.x > platform.
                position.x + platform.scale.x)
            {
                // Collision happened from the right side of the player.
                m_position.x = platform.position.x + platform.scale.x;
                m_velocity.x = 0;
            }

            m_on_platform = true;
        }
    }
}

[[nodiscard]] glm::vec2 Player::GetPosition() const
{
    return m_position;
}

glm::vec2 Player::GetScale() const
{
    return m_scale;
}

void Player::SetWeaponRotation(const float rotation)
{
    m_weapon_rotation = rotation;
}

float Player::GetWeaponRotation() const
{
    return m_weapon_rotation;
}

void Player::SetId(const unsigned int id)
{
    m_id = id;
}

unsigned int Player::GetId() const
{
    return m_id;
}

bool Player::IsGrounded() const
{
    return m_position.y < GROUND_HEIGHT + PLAYER_SCALE.y + 0.01f || m_on_platform;
}
