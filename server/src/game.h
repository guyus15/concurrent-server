#pragma once

#include <glm/vec2.hpp>

#include <vector>

class Projectile;

/**
 * \brief The server game class.
 */
class Game
{
public:
    ~Game() = default;

    Game(const Game&) = default;
    Game& operator=(const Game&) = default;

    Game(Game&&) noexcept = default;
    Game& operator=(Game&&) noexcept = default;

    static void Initialise();
    static void Update(double dt);
    static void SpawnProjectile(glm::vec2 position, glm::vec2 direction);
    static [[nodiscard]] std::vector<Projectile> GetProjectiles();

private:
    std::vector<Projectile> m_projectiles;

    Game() = default;

    static Game s_instance;
    static Game& Get();
};