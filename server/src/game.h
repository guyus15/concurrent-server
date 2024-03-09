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

    /**
     * \brief Updates the game.
     * \param dt The delta time between frames of the game.
     */
    static void Update(double dt);

    /**
     * \brief Spawns an instance of a projectile in the game world.
     * \param position The position of the new projectile.
     * \param direction The direction of the new projectile.
     * \param src_id The identifier of the player who initialised the spawning of this projectile.
     */
    static void SpawnProjectile(glm::vec2 position, glm::vec2 direction, unsigned int src_id);

    /**
     * \brief Gets the projectiles which currently exist in the game world.
     * \return A vector of existing projectiles.
     */
    static [[nodiscard]] std::vector<Projectile> GetProjectiles();

private:
    std::vector<Projectile> m_projectiles;

    Game() = default;

    static Game s_instance;
    static Game& Get();
};