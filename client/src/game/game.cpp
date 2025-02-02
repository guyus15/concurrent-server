#include "game.h"
#include "input.h"

#include "ecs/components.h"

#include <common/level_manager.h>
#include <common/world.h>

#include <common/assets/asset_manager.h>

#include <common/events/event_manager.h>

#include <common/utils/logging.h>

#include "client.h"

Game Game::s_instance{};

void Game::Initialise()
{
    EventManager::AddListener<OnChatVisibleEvent>(OnChatVisible);

    Get().m_scene = std::make_unique<Scene>();

    Get().m_camera.Initialise();

    LevelManager::Initialise();

    const auto& renderable_content = LevelManager::GetActive().GetRenderableContent();
    for (size_t i = 0; i < renderable_content.size(); i++)
    {
        Entity new_level_content_entity = Get().m_scene->CreateEntity("renderable_content" + std::to_string(i));

        auto& [transform] = new_level_content_entity.AddComponent<TransformComponent>();
        transform.position = renderable_content[i].position;
        transform.scale = renderable_content[i].scale;

        auto& [sprite, colour] = new_level_content_entity.AddComponent<SpriteRendererComponent>();
        Texture2d content_tex;
        switch (renderable_content[i].type)
        {
        case LevelContent::Type::Platform:
            // Temporary texture, this will be updated to a platform texture.
            content_tex = AssetManager<Texture2d>::LoadOrRetrieve("resources/textures/generic.png");
            break;
        case LevelContent::Type::Wall:
            // Temporary texture, this will be updated to a wall texture.
            content_tex = AssetManager<Texture2d>::LoadOrRetrieve("resources/textures/generic.png");
            break;
        default:
            // Default texture is the texture of the platform.
            content_tex = AssetManager<Texture2d>::LoadOrRetrieve("resources/textures/generic.png");
        }
        sprite = std::make_unique<Sprite>(content_tex);
        colour = { 0.6f, 0.6f, 0.6f };

        Get().m_level_content.push_back(new_level_content_entity);
    }
}

void Game::Update(const double dt)
{
    Get().m_scene->Update(dt);

    if (!Get().m_players.contains(0))
        return;

    if (!Get().m_player_weapons.contains(0))
        return;

    // Handle local player weapon rotation.
    const glm::vec2 mouse_pos = Input::GetMousePosition();
    const glm::vec2 mouse_pos_world = GetMousePositionToWorldSpace(mouse_pos);

    const glm::vec2& local_player_pos = Get().m_players[0].GetComponent<TransformComponent>().transform.position;
    const glm::vec2 weapon_direction = glm::normalize(mouse_pos_world - local_player_pos);
    const float rotation = atan2(weapon_direction.y, weapon_direction.x);

    SetPlayerWeaponRotation(0, rotation);
}

void Game::UpdateCamera()
{
    Get().m_camera.CalculateMatrices();
}

void Game::SpawnPlayer(const unsigned id, const std::string& name, const Transform& transform)
{
    // Check a player with this ID doesn't already exist.
    if (Get().m_players.contains(id))
    {
        SCX_CORE_ERROR("Player with ID {0} already exists.", id);
        return;
    }

    Entity new_player_entity = Get().m_scene->CreateEntity(name);

    auto& [player_transform] = new_player_entity.AddComponent<TransformComponent>();
    player_transform.position = transform.position;
    player_transform.scale = transform.scale;

    auto& [sprite, colour] = new_player_entity.AddComponent<SpriteRendererComponent>();
    const auto player_tex = AssetManager<Texture2d>::LoadOrRetrieve("resources/textures/player.png");
    sprite = std::make_unique<Sprite>(player_tex);
    colour = { 1.0f, 0.0f, 0.0f }; // Set the enemy player's colour to red.

    Get().m_players[id] = new_player_entity;

    // Also spawn the player's weapon
    SpawnPlayerWeapon(id, transform.position);
}

void Game::SpawnLocalPlayer(const std::string& name, const Transform& transform)
{
    Entity new_player_entity = Get().m_scene->CreateEntity(name);

    auto& [player_transform] = new_player_entity.AddComponent<TransformComponent>();
    player_transform.position = transform.position;
    player_transform.scale = transform.scale;

    auto& [sprite, colour] = new_player_entity.AddComponent<SpriteRendererComponent>();
    const auto player_tex = AssetManager<Texture2d>::LoadOrRetrieve("resources/textures/player.png");
    sprite = std::make_unique<Sprite>(player_tex);
    colour = { 0.0f, 0.0f, 1.0f }; // Set the enemy player's colour to blue.

    Get().m_players[0] = new_player_entity;

    // Also spawn the player's weapon
    SpawnPlayerWeapon(0, transform.position);

    Get().m_is_local_player_alive = true;
}

void Game::SpawnPlayerWeapon(const unsigned int id, const glm::vec2& position)
{
    Entity new_weapon_entity = Get().m_scene->CreateEntity("PlayerWeapon" + std::to_string(id));

    auto& [weapon_transform] = new_weapon_entity.AddComponent<TransformComponent>();
    weapon_transform.position = { position.x - WEAPON_SCALE.y, position.y - WEAPON_SCALE.y };
    weapon_transform.scale = WEAPON_SCALE;
    weapon_transform.rotation = 0.0f;

    auto& [sprite, colour] = new_weapon_entity.AddComponent<SpriteRendererComponent>();
    const auto weapon_tex = AssetManager<Texture2d>::LoadOrRetrieve("resources/textures/generic.png");
    sprite = std::make_unique<Sprite>(weapon_tex);
    colour = { 0.0f, 0.0f, 0.0f };

    Get().m_player_weapons[id] = new_weapon_entity;
}

void Game::RemovePlayer(const unsigned int id)
{
    const auto player_it = Get().m_players.find(id);
    if (player_it == Get().m_players.end())
    {
        SCX_CORE_ERROR("Could not find a player entity associated with the ID {0}.", id);
        return;
    }

    Entity& player_entity = Get().m_players[id];
    Get().m_scene->DeleteEntity(player_entity);

    Get().m_players.erase(player_it);

    // Also remove the player's weapon
    const auto weapon_it = Get().m_player_weapons.find(id);
    if (weapon_it == Get().m_player_weapons.end())
    {
        SCX_CORE_ERROR("Could not find a player weapon entity associated with the ID {0}.", id);
        return;
    }

    Entity& player_weapon_entity = Get().m_player_weapons[id];
    Get().m_scene->DeleteEntity(player_weapon_entity);

    Get().m_player_weapons.erase(weapon_it);
}

void Game::RemoveLocalPlayer()
{
    RemovePlayer(0);
    Get().m_is_local_player_alive = false;
}

void Game::SetPlayerPosition(const unsigned int id, const glm::vec2& position)
{
    // There is a chance that we are trying to update a position of a player which has not yet
    // been spawned yet, so if we can't find it in the player map, just return.
    if (!Get().m_players.contains(id))
        return;

    Entity& player_entity = Get().m_players[id];
    auto& [transform] = player_entity.GetComponent<TransformComponent>();
    transform.position = position;

    // Also update the position of the player's weapon.
    SetPlayerWeaponPosition(id, transform.position);
}

void Game::SetLocalPlayerHealth(const int health)
{
    Get().m_local_player_current_health = health;
}

void Game::KillPlayer(const unsigned int id)
{
    if (id == Client::GetClientId())
    {
        RemoveLocalPlayer();

        OnLocalPlayerDeathEvent local_player_death_evt{};
        EventManager::Broadcast(local_player_death_evt);
    }
    else
        RemovePlayer(id);
}

void Game::SetPlayerWeaponRotation(const unsigned id, const float rotation)
{
    // If the player's weapon does not yet exist, just return.
    if (!Get().m_player_weapons.contains(id))
        return;

    Entity& player_weapon_entity = Get().m_player_weapons[id];
    auto& [transform] = player_weapon_entity.GetComponent<TransformComponent>();
    transform.rotation = rotation;
}

float Game::GetLocalPlayerWeaponRotation()
{
    // If local player weapon data does not yet exist, just return a default value.
    if (!Get().m_player_weapons.contains(0))
        return 0.0f;

    Entity& player_weapon_entity = Get().m_player_weapons[0];
    return player_weapon_entity.GetComponent<TransformComponent>().transform.rotation;
}

void Game::UpdateProjectile(const UUID id, const glm::vec2 position, const float rotation)
{
    // Check if a projectile with this identifier exists. If it doesn't
    // spawn one in.
    const auto it = Get().m_projectiles.find(id);
    if (it == Get().m_projectiles.end())
    {
        SpawnProjectile(id, position, rotation);
        return;
    }

    Entity& projectile_entity = it->second;

    auto& [transform] = projectile_entity.GetComponent<TransformComponent>();
    transform.position = { position.x, position.y };
    transform.rotation = rotation;
}

void Game::DestroyProjectile(const UUID id)
{
    // Check if a projectile with this identifier exists. If it doesn't
    // ignore the destruction of it.
    const auto it = Get().m_projectiles.find(id);
    if (it == Get().m_projectiles.end())
        return;

    Entity& projectile_entity = Get().m_projectiles[id];
    Get().m_scene->DeleteEntity(projectile_entity);

    Get().m_projectiles.erase(it);
}

void Game::SpawnProjectile(const UUID id, const glm::vec2 position, const float rotation)
{
    Entity new_projectile = Get().m_scene->CreateEntity("Projectile" + std::to_string(static_cast<uint64_t>(id)));

    auto& [projectile_transform] = new_projectile.AddComponent<TransformComponent>();
    projectile_transform.position = { position.x - PROJECTILE_SCALE.y, position.y - PROJECTILE_SCALE.y };
    projectile_transform.scale = PROJECTILE_SCALE;
    projectile_transform.rotation = rotation;

    auto& [sprite, colour] = new_projectile.AddComponent<SpriteRendererComponent>();
    const auto weapon_tex = AssetManager<Texture2d>::LoadOrRetrieve("resources/textures/generic.png");
    sprite = std::make_unique<Sprite>(weapon_tex);
    colour = { 0.5f, 0.5f, 0.1f };

    Get().m_projectiles[id] = new_projectile;
}

void Game::OnChatVisible(GameEvent& evt)
{
    const auto& on_chat_visible_event = dynamic_cast<OnChatVisibleEvent&>(evt);

    Get().m_is_chat_window_open = on_chat_visible_event.visible;
}

OrthographicCamera& Game::GetCamera()
{
    return Get().m_camera;
}

bool Game::ShouldSendInput()
{
    return Get().m_is_local_player_alive && !Get().m_is_chat_window_open;
}

Game::Game()
    : m_camera{},
      m_local_player_current_health{},
      m_is_local_player_alive{ false },
      m_is_chat_window_open{ false }
{
}

void Game::SetPlayerWeaponPosition(const unsigned id, const glm::vec2& position)
{
    // If the player's weapon does not exist yet, just return.
    if (!Get().m_player_weapons.contains(id))
        return;

    Entity& player_weapon_entity = Get().m_player_weapons[id];
    auto& [transform] = player_weapon_entity.GetComponent<TransformComponent>();
    transform.position = position;
}

Game& Game::Get()
{
    return s_instance;
}
