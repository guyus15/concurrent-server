#include "game.h"

#include "ecs/components.h"

#include <common/utils/logging.h>

#include "asset_manager.h"

Game Game::s_instance{};

void Game::Initialise()
{
    Get().m_scene = std::make_unique<Scene>();
}

void Game::Update(const double dt)
{
    Get().m_scene->Update(dt);
}

void Game::SpawnPlayer(const unsigned id, const std::string& name, const glm::vec2& position)
{
    // Check a player with this ID doesn't already exist.
    if (Get().m_players.contains(id))
    {
        SCX_CORE_ERROR("Player with ID {0} already exists.", id);
        return;
    }

    Entity new_player_entity = Get().m_scene->CreateEntity(name);

    auto& [transform] = new_player_entity.AddComponent<TransformComponent>();
    transform.position = { position.x, position.y };

    auto& [sprite, colour] = new_player_entity.AddComponent<SpriteRendererComponent>();
    const auto player_tex = AssetManager<Texture2d>::LoadOrRetrieve("resources/textures/player.png");
    sprite = std::make_unique<Sprite>(player_tex);
    colour = { 1.0f, 0.0f, 0.0f }; // Set the enemy player's colour to red.

    Get().m_players[id] = new_player_entity;
}

void Game::SpawnLocalPlayer(const std::string& name, const glm::vec2& position)
{
    // TODO: Rewrite this to avoid code duplication.

    Entity new_player_entity = Get().m_scene->CreateEntity(name);

    auto& [transform] = new_player_entity.AddComponent<TransformComponent>();
    transform.position = position;

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

Game& Game::Get()
{
    return s_instance;
}
