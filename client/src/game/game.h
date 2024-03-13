#pragma once

#include "ecs/entity.h"

#include "rendering/transform.h"
#include "rendering/camera.h"

#include <common/events/events.h>

#include <common/utils/uuid.h>

#include <glm/vec2.hpp>

#include <memory>
#include <unordered_map>
#include <string>

class UUID;

/**
 * \brief The client game class.
 */
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
     * \brief Removes the local player from the game world.
     */
    static void RemoveLocalPlayer();

    /**
     * \brief Sets the position of the player with the given ID.
     * \param id The ID of the player whose position to update.
     * \param position The new position of the player.
     */
    static void SetPlayerPosition(unsigned int id, const glm::vec2& position);

    /**
     * \brief Sets the health value of the local player.
     * \param health The new health value.
     */
    static void SetLocalPlayerHealth(int health);

    /**
     * \brief Kills the player associated with the given identifier.
     * \param id The identifier of the player to kill.
     */
    static void KillPlayer(unsigned int id);

    /**
     * \brief Sets the rotation of a player's weapon.
     * \param id The identifier of the player whose weapon rotation is to be updated.
     * \param rotation The new rotation of the player's weapon.
     */
    static void SetPlayerWeaponRotation(unsigned int id, float rotation);

    /**
     * \brief Gets the local player's current weapon rotation.
     * \return The rotation of the local player's current weapon.
     */
    static [[nodiscard]] float GetLocalPlayerWeaponRotation();

    /**
     * \brief Updates a projectile with the given identifier with the given position and rotation.
     * \param id The identifier of the projectile to update.
     * \param position The new position to update the projectile with.
     * \param rotation The new rotation to update the projectile with.
     */
    static void UpdateProjectile(UUID id, glm::vec2 position, float rotation);

    /**
     * \brief Destroys an instance of a projectile.
     * \param id The identifier of the projectile to destroy.
     */
    static void DestroyProjectile(UUID id);

    /**
     * \brief Gets a reference to the game's camera.
     * \return The game's camera reference.
     */
    static [[nodiscard]] OrthographicCamera& GetCamera();

    /**
     * \brief Determines whether input should be sent from the client to the server.
     * \return A true or false value indicating whether or not input should be sent to
     * the server.
     */
    static [[nodiscard]] bool ShouldSendInput();

private:
    std::unique_ptr<Scene> m_scene;
    OrthographicCamera m_camera;
    std::unordered_map<unsigned int, Entity> m_players;
    std::unordered_map<unsigned int, Entity> m_player_weapons;
    std::unordered_map<UUID, Entity> m_projectiles;
    std::vector<Entity> m_level_content;
    int m_local_player_current_health;
    bool m_is_local_player_alive;
    bool m_is_chat_window_open;

    Game();

    /**
     * \brief Sets the position of a player's weapon.
     * \param id The identifier of the player whose weapon position is to be updated.
     * \param position The new position of the player's weapon.
     */
    static void SetPlayerWeaponPosition(unsigned int id, const glm::vec2& position);

    /**
     * \brief Spawns a projectile within the game world.
     * \param id The identifier of the new projectile.
     * \param position The position of the new projectile.
     * \param rotation The rotation of the new projectile.
     */
    static void SpawnProjectile(UUID id, glm::vec2 position, float rotation);

    static void OnChatVisible(GameEvent& evt);

    static Game s_instance;
    static Game& Get();
};
