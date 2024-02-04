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

    /**
     * \brief Updates the necessary physics required for player movement.
     * \param dt The delta time.
     */
    void Update(double dt);

    /**
     * \brief Adds the given value of health to this player.
     * \param amount The amount of health to add to this player.
     */
    void AddHealth(int amount);

    /**
     * \brief Removes the given value of health from this player.
     * \param amount The amount of health to remove from this player.
     */
    void RemoveHealth(int amount);

    /**
     * \brief Gets the current health of the this player.
     * \return The current health of this player.
     */
    [[nodiscard]] int GetCurrentHealth() const;

    /**
     * \brief Processes the inputs from this player's respective client.
     * \param key_pressed_down_w The input representing the W key being pressed down.
     * \param key_pressed_a The input representing the A key being pressed.
     * \param key_pressed_d The input representing the D key being pressed.
     */
    void ProcessInput(bool key_pressed_down_w, bool key_pressed_a, bool key_pressed_d);

    /**
     * \brief Gets the current position of this player.
     * \return The current position of this player.
     */
    [[nodiscard]] glm::vec2 GetPosition() const;

    /**
     * \brief Gets the current scale of this player.
     * \return The current scale of this player.
     */
    [[nodiscard]] glm::vec2 GetScale() const;

private:
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    glm::vec2 m_scale;
    int m_health;

    /**
     * \brief Determines whether the player is currently grounded.
     * \return A true or false value indicating whether or not the player is
     * currently grounded.
     */
    [[nodiscard]] bool IsGrounded() const;
};
