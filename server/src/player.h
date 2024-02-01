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

    void Update(double dt);

    void AddHealth(int amount);
    void RemoveHealth(int amount);
    [[nodiscard]] int GetCurrentHealth() const;

    void ProcessInput(bool key_pressed_down_w, bool key_pressed_a, bool key_pressed_d);
    [[nodiscard]] glm::vec2 GetPosition() const;

private:
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    int m_health;
};
