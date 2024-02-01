#include "player.h"

constexpr glm::vec2 PLAYER_START_POSITION = { 0.0f, 0.0f };
constexpr int PLAYER_MAX_HEALTH = 100;
constexpr float PLAYER_MOVEMENT_SPEED = 1.0f,
                PLAYER_JUMP_SPEED = 3.0f,
                PLAYER_DAMPENING_FACTOR = 0.1f,
                PLAYER_GRAVITY_FACTOR = 9.81f;

Player::Player()
    : m_position{ PLAYER_START_POSITION },
      m_velocity{ 0.0f, 0.0f },
      m_health{ PLAYER_MAX_HEALTH }
{
}

void Player::Update(const double dt)
{
    m_position += m_velocity * static_cast<float>(dt);

    m_velocity.y -= PLAYER_GRAVITY_FACTOR * static_cast<float>(dt);

    if (m_position.y <= 0)
        m_velocity.y = 0;

    // Gradually bring the x velocity to zero.
    m_velocity = { m_velocity.x * PLAYER_DAMPENING_FACTOR, m_velocity.y };
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
    if (key_pressed_down_w && IsGrounded())
        m_velocity.y = PLAYER_JUMP_SPEED;

    if (key_pressed_a)
        m_velocity.x = -PLAYER_MOVEMENT_SPEED;
    if (key_pressed_d)
        m_velocity.x = PLAYER_MOVEMENT_SPEED;
}

[[nodiscard]] glm::vec2 Player::GetPosition() const
{
    return m_position;
}

bool Player::IsGrounded() const
{
    // This for now will just determine if the player's Y position is at 0 (the ground).
    // In future this function will contain more elaborate checking to determine multiple
    // ground levels on the Y axis.

    return m_position.y < 0.1f;
}
