#pragma once

#include <glm/vec2.hpp>

class Player
{
public:
    Player();
    ~Player() = default;

    Player(const Player&) = default;
    Player& operator=(const Player&) = default;

    Player(Player&&) noexcept = default;
    Player& operator=(Player&&) noexcept = default;

    void AddHealth(int amount);
    void RemoveHealth(int amount);
    [[nodiscard]] int GetCurrentHealth() const;

    void SetPosition(glm::vec2 position);
    [[nodiscard]] glm::vec2 GetPosition() const;

private:
    glm::vec2 m_position;
    int m_health;
};

