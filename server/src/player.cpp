#include "player.h"

constexpr glm::vec2 PLAYER_START_POSITION = { 0.0f, 0.0f };
constexpr int PLAYER_MAX_HEALTH = 100;

Player::Player()
    : m_position{ PLAYER_START_POSITION },
      m_health{ PLAYER_MAX_HEALTH }
{
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

void Player::SetPosition(const glm::vec2 position)
{
    m_position = position;
}

[[nodiscard]] glm::vec2 Player::GetPosition() const
{
    return m_position;
}
