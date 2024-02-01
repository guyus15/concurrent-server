#include "player.h"

constexpr glm::vec2 PLAYER_START_POSITION = { 0.0f, 0.0f };
constexpr int PLAYER_MAX_HEALTH = 100;
constexpr float PLAYER_MOVEMENT_SPEED = 1.0f;
constexpr float PLAYER_DAMPENING_FACTOR = 0.1f;

Player::Player()
    : m_position{ PLAYER_START_POSITION },
      m_velocity{ 0.0f, 0.0f },
      m_health{ PLAYER_MAX_HEALTH }
{
}

void Player::Update(const double dt)
{
    m_position += m_velocity * PLAYER_MOVEMENT_SPEED * static_cast<float>(dt);

    // Gradually bring the velocity to zero.
    m_velocity *= PLAYER_DAMPENING_FACTOR;
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
    if (key_pressed_down_w)
    {
        // Jump
    }

    if (key_pressed_a)
        m_velocity.x = -1.0f;
    if (key_pressed_d)
        m_velocity.x = 1.0f;
}

[[nodiscard]] glm::vec2 Player::GetPosition() const
{
    return m_position;
}
