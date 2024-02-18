#pragma once

#include "ecs/entity.h"

#include "rendering/transform.h"
#include "rendering/camera.h"

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
     * \brief Updates the game's camera projection matrix.
     */
    static void UpdateCamera();

    /**
     * \brief Spawns an instance of a player in the game world.
     * \param id The ID of the client associated with this player.
     * \param name The name of this player.
     * \param transform A transform specifying details on how to spawn the player.
     */
    static void SpawnPlayer(unsigned int id, const std::string& name, const Transform& transform);

    /**
     * \brief Spawns a local instance of a player in the game world.
     * \param name The name of this player.
     * \param transform A transform specifying details on how to spawn the player.
     */
    static void SpawnLocalPlayer(const std::string& name, const Transform& transform);

    /**
     * \brief Spawns a weapon in a player's hand.
     * \param id The identifier of the player whose hands put to spawn the weapon.
     * \param position The position in which to spawn the weapon.
     */
    static void SpawnPlayerWeapon(unsigned int id, const glm::vec2& position);

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

    /**
     * \brief Gets a reference to the game's camera.
     * \return The game's camera reference.
     */
    static [[nodiscard]] OrthographicCamera& GetCamera();

private:
    std::unique_ptr<Scene> m_scene;
    OrthographicCamera m_camera;
    std::unordered_map<unsigned int, Entity> m_players;
    std::unordered_map<unsigned int, Entity> m_player_weapons;
    std::vector<Entity> m_level_content;

    Game();

    /**
     * \brief Sets the position of a player's weapon.
     * \param id The identifier of the player whose weapon position is to be updated.
     * \param position The new position of the player's weapon.
     */
    static void SetPlayerWeaponPosition(unsigned int id, const glm::vec2& position);

    /**
     * \brief Sets the rotation of a player's weapon.
     * \param id The identifier of the player whose weapon rotation is to be updated.
     * \param rotation The new rotation of the player's weapon.
     */
    static void SetPlayerWeaponRotation(unsigned int id, float rotation);

    static Game s_instance;
    static Game& Get();
};
