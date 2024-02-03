#pragma once

#include "ecs/entity.h"

#include <glm/vec2.hpp>

#include <memory>
#include <unordered_map>
#include <string>

class Game
{
public:
    ~Game() = default;

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    Game(Game&&) noexcept = delete;
    Game& operator=(Game&&) noexcept = delete;

    static void Initialise();

    /**
     * \brief Updates the game.
     * \param dt The delta time between frames of the game.
     */
    static void Update(double dt);

    /**
     * \brief Spawns an instance of a player in the game world.
     * \param id The ID of the client associated with this player.
     * \param name The name of this player.
     * \param position The position in which to spawn the player.
     */
    static void SpawnPlayer(unsigned int id, const std::string& name, const glm::vec2& position);

    /**
     * \brief Spawns a local instance of a player in the game world.
     * \param name The name of this player.
     * \param position The position in which to spawn the player.
     */
    static void SpawnLocalPlayer(const std::string& name, const glm::vec2& position);

    /**
     * \brief Removes a player from the game world.
     * \param id The ID of the client associated with the player to remove.
     */
    static void RemovePlayer(unsigned int id);

    /**
     * \brief Sets the position of the player with the given ID.
     * \param id The ID of the player whose position to update.
     * \param position The new position of the player.
     */
    static void SetPlayerPosition(unsigned int id, const glm::vec2& position);

private:
    std::unique_ptr<Scene> m_scene;
    std::unordered_map<unsigned int, Entity> m_players;

    Game() = default;

    static Game s_instance;
    static Game& Get();
};