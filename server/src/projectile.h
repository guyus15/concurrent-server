#pragma once

#include <glm/vec2.hpp>

class Projectile
{
public:
    Projectile(glm::vec2 position, glm::vec2 direction, unsigned int from_client);
    ~Projectile() = default;

    Projectile(const Projectile&) = default;
    Projectile& operator=(const Projectile&) = default;

    Projectile(Projectile&&) noexcept = default;
    Projectile& operator=(Projectile&&) noexcept = default;

    void Update(double dt);
    
private:
    unsigned int m_from_client;
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    float m_rotation;
};