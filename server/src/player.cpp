#include "player.h"

#include <common/level_manager.h>
#include <common/world.h>

#include <common/utils/logging.h>

constexpr glm::vec2 PLAYER_START_POSITION = { 0.0f, 0.0f },
                    PLAYER_SCALE{ 10.0f, 10.0f };
constexpr float GRAVITY = 9.81f,
                GRAVITY_SCALE = 10.0f,
                PLAYER_MOVEMENT_SPEED = 50.0f,
                PLAYER_JUMP_SPEED = 100.0f,
                PLAYER_DAMPENING_FACTOR = 0.1f,
                PLAYER_SNAP_TO_GROUND_DISTANCE = 0.001f,
                GROUND_HEIGHT = -(WORLD_DIMENSIONS_Y / 2) + PLAYER_SCALE.y;
constexpr int PLAYER_MAX_HEALTH = 100;

static bool Collision(glm::vec2 position1, glm::vec2 scale1, glm::vec2 position2, glm::vec2 scale2);

Player::Player()
    : m_position{ PLAYER_START_POSITION },
      m_velocity{ 0.0f, 0.0f },
      m_scale{ PLAYER_SCALE },
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

    m_velocity.y -= GRAVITY * GRAVITY_SCALE * static_cast<float>(dt);

    if (m_position.y <= GROUND_HEIGHT + PLAYER_SNAP_TO_GROUND_DISTANCE)
    {
        m_velocity.y = 0;
        m_position.y = GROUND_HEIGHT;
    }

    // Gradually bring the x velocity to zero.
    m_velocity = { m_velocity.x * PLAYER_DAMPENING_FACTOR, m_velocity.y };

    HandleCollisions();
}

void Player::AddHealth(const int amount)
{
    if ((m_health += amount) > PLAYER_MAX_HEALTH)
        m_health = PLAYER_MAX_HEALTH;
}

void Player::RemoveHealth(const int amount)
{
    if ((m_health -= amount) < 0)
        m_health = 0;
}

int Player::GetCurrentHealth() const
{
    return m_health;
}

void Player::ProcessInput(const bool key_pressed_down_w, const bool key_pressed_a, const bool key_pressed_d)
{
    // Acquire lock on player guard.
    std::unique_lock guard_lock{ m_guard };

    m_jumping = false;

    if (key_pressed_down_w && IsGrounded())
    {
        m_velocity.y = PLAYER_JUMP_SPEED;
        m_on_platform = false;
    }

    if (key_pressed_a)
        m_velocity.x = -PLAYER_MOVEMENT_SPEED;
    if (key_pressed_d)
        m_velocity.x = PLAYER_MOVEMENT_SPEED;
}

void Player::HandleCollisions()
{
    Level& current_level = LevelManager::GetActive();

    for (const auto& platform : current_level.GetByType(LevelContent::Type::Platform))
    {
        if (Collision(m_position, m_scale, platform.position, platform.scale))
        {
            if (m_position.y + m_scale.y > platform.position.y && m_position.y < platform.position.y)
            {
                // Collision happened from the top of the player.
                m_position.y = platform.position.y - m_scale.y;
                m_velocity.y = 0;
            }
            else if (m_position.y < platform.position.y + platform.scale.y && m_position.y + m_scale.y > platform.position.y + platform.scale.y)
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
            else if (m_position.x < platform.position.x + platform.scale.x && m_position.x + m_scale.x > platform.position.x + platform.scale.x)
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

bool Player::IsGrounded() const
{
    return m_position.y < GROUND_HEIGHT + 0.01f || m_on_platform;
}

bool Collision(const glm::vec2 position1, const glm::vec2 scale1, const glm::vec2 position2, const glm::vec2 scale2)
{
    return position1.x < position2.x + scale2.x &&
        position1.x + scale1.x > position2.x &&
        position1.y < position2.y + scale2.y &&
        position1.y + scale1.y > position2.y;
}
