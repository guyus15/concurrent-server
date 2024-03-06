#pragma once

#include <common/utils/clock.h>

#include <glm/vec2.hpp>

#include <mutex>

class Player
{
public:
    Player();
    ~Player() = default;

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    Player(Player&&) noexcept = delete;
    Player& operator=(Player&&) noexcept = delete;

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
     * \param left_mouse_btn_pressed The input representing the left mouse button being pressed.
     */
    void ProcessInput(bool key_pressed_down_w, bool key_pressed_a, bool key_pressed_d, bool left_mouse_btn_pressed);

    /**
     * \brief Handles the collisions between the player and other entities in the world
     * such as Platforms, Walls, etc.
     */
    void HandleCollisions();

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

    /**
     * \brief Sets the player's weapon rotation.
     * \param rotation The new player weapon rotation.
     */
    void SetWeaponRotation(float rotation);

    /**
     * \brief Gets the player's weapon rotation.
     * \return The player's weapon rotation.
     */
    [[nodiscard]] float GetWeaponRotation() const;

    void SetId(unsigned int id);

    [[nodiscard]] unsigned int GetId() const;

private:
    unsigned int m_id;
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    glm::vec2 m_scale;
    float m_weapon_rotation;
    Clock m_weapon_clock;
    bool m_on_platform;
    int m_health;
    std::mutex m_guard;

    /**
     * \brief Determines whether the player is currently grounded.
     * \return A true or false value indicating whether or not the player is
     * currently grounded.
     */
    [[nodiscard]] bool IsGrounded() const;
};
