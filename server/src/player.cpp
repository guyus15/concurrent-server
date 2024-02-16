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

    if (key_pressed_down_w && IsGrounded())
        m_velocity.y = PLAYER_JUMP_SPEED;

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
        if (m_position.x < platform.position.x + platform.scale.x &&
            m_position.x + m_scale.x > platform.position.x &&
            m_position.y < platform.position.y + platform.scale.y &&
            m_position.y + m_scale.y > platform.position.y)
        {
            const float y_diff_up = m_position.y + m_scale.y - platform.position.y;
            const float y_diff_down = platform.position.y - m_position.y + m_scale.y;

            if (abs(y_diff_up) <= abs(y_diff_down))
                m_position.y -= y_diff_up;
            else
                m_position.y += y_diff_down;

            m_on_platform = true;
            m_velocity.y = 0;
        }
        else
            m_on_platform = false;
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
    // This for now will just determine if the player's Y position is at 0 (the ground).
    // In future this function will contain more elaborate checking to determine multiple
    // ground levels on the Y axis.

    return m_position.y < GROUND_HEIGHT + 0.01f || m_on_platform;
}
