#include "game.h"

#include "ecs/components.h"

#include <common/assets/asset_manager.h>

#include <common/level_manager.h>

#include <common/utils/logging.h>

Game Game::s_instance{};

void Game::Initialise()
{
    Get().m_scene = std::make_unique<Scene>();

    LevelManager::Initialise();

    const auto renderable_content = LevelManager::GetActive().GetRenderableContent();
    for (size_t i = 0; i < renderable_content.size(); i++)
    {
        Entity new_level_content_entity = Get().m_scene->CreateEntity("renderable_content" + std::to_string(i));

        auto& [transform] = new_level_content_entity.AddComponent<TransformComponent>();
        transform.position = renderable_content[i].position;
        transform.scale = renderable_content[i].scale;

        auto& [sprite, _] = new_level_content_entity.AddComponent<SpriteRendererComponent>();
        Texture2d content_tex;
        switch (renderable_content[i].type)
        {
        case LevelContent::Type::Platform:
            // Temporary texture, this will be updated to a platform texture.
            content_tex = AssetManager<Texture2d>::LoadOrRetrieve("resources/textures/player.png");
            break;
        case LevelContent::Type::Wall:
            // Temporary texture, this will be updated to a wall texture.
            content_tex = AssetManager<Texture2d>::LoadOrRetrieve("resources/textures/player.png");
            break;
        default:
            // Default texture is the texture of the platform.
            content_tex = AssetManager<Texture2d>::LoadOrRetrieve("resources/textures/player.png");
        }
        sprite = std::make_unique<Sprite>(content_tex);

        Get().m_level_content.push_back(new_level_content_entity);
    }
}

void Game::Update(const double dt)
{
    Get().m_scene->Update(dt);
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
}

void Game::SpawnLocalPlayer(const std::string& name, const Transform& transform)
{
    // TODO: Rewrite this to avoid code duplication.

    Entity new_player_entity = Get().m_scene->CreateEntity(name);

    auto& [player_transform] = new_player_entity.AddComponent<TransformComponent>();
    player_transform.position = transform.position;
    player_transform.scale = transform.scale;

    auto& [sprite, colour] = new_player_entity.AddComponent<SpriteRendererComponent>();
    const auto player_tex = AssetManager<Texture2d>::LoadOrRetrieve("resources/textures/player.png");
    sprite = std::make_unique<Sprite>(player_tex);
    colour = { 0.0f, 0.0f, 1.0f }; // Set the enemy player's colour to blue.

    Get().m_players[0] = new_player_entity;
}

void Game::RemovePlayer(const unsigned int id)
{
    const auto it = Get().m_players.find(id);
    if (it == Get().m_players.end())
    {
        SCX_CORE_ERROR("Could not find a player entity associated with the ID {0}.", id);
        return;
    }

    Entity& player_entity = Get().m_players[id];
    Get().m_scene->DeleteEntity(player_entity);

    Get().m_players.erase(it);
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
}

Game& Game::Get()
{
    return s_instance;
}
