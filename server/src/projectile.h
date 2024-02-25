#pragma once

#include <glm/vec2.hpp>

#include <common/utils/uuid.h>
#include <common/utils/clock.h>

class Projectile
{
public:
    Projectile(glm::vec2 position, glm::vec2 direction);
    ~Projectile() = default;

    Projectile(const Projectile&) = default;
    Projectile& operator=(const Projectile&) = default;

    Projectile(Projectile&&) noexcept = default;
    Projectile& operator=(Projectile&&) noexcept = default;

    /**
     * \brief Updates the projectile's physics
     * \param dt The delta time between the current frame and the last frame.
     */
    void Update(double dt);

    /**
     * \brief Gets the identifier of this projectile.
     * \return The projectile's identifier.
     */
    [[nodiscard]] UUID GetId() const;

    /**
     * \brief Gets the position of this projectile.
     * \return The projectile's position.
     */
    [[nodiscard]] glm::vec2 GetPosition() const;

    /**
     * \brief Gets the rotation of this projectile.
     * \return The projectile's rotation.
     */
    [[nodiscard]] float GetRotation() const;

private:
    UUID m_id;
    Clock m_birth_clock;
    bool m_has_died;
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    float m_rotation;
};
