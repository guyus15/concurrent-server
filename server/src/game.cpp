#include "game.h"
#include "player.h"
#include "projectile.h"
#include "server.h"

#include <common/level_manager.h>

Game Game::s_instance{};

void Game::Initialise()
{
    LevelManager::Initialise();
}

void Game::Update(const double dt)
{
    // Update and send player movement packets to connected clients.
    for (auto& [client_id, client_info] : Server::GetClientInfoMap())
    {
        Player& client_player = client_info.player;
        client_player.Update(dt);

        PlayerMovement(client_id, client_player);
    }

    // Update and send projectile-related packets to connected clients.
    auto it = Get().m_projectiles.begin();
    while (it != Get().m_projectiles.end())
    {
        if ((*it).HasExpired())
        {
            // If a projectile has expired, notify the clients and remove it.
            ProjectileDestroy(*it);
            it = Get().m_projectiles.erase(it);
        }
        else
        {
            (*it).Update(dt);
            ++it;
        }
    }
}

void Game::SpawnProjectile(const glm::vec2 position, const glm::vec2 direction, const unsigned int src_id)
{
    const Projectile projectile = Get().m_projectiles.emplace_back(position, direction, src_id);
    ProjectileUpdate(projectile);
}

void Game::SpawnPlayer(Player& player)
{
    Level& current_level = LevelManager::GetActive();
    const auto& potential_spawn_points = current_level.GetByType(LevelContent::Type::PlayerSpawnPoint);

    if (potential_spawn_points.empty())
    {
        SCX_CORE_WARN("No spawn points exist for this level, using default player spawn location.");
        player.SetPosition({ 0, 0 });
        return;
    }

    const LevelContent spawn_point = potential_spawn_points[0];
    player.SetPosition(spawn_point.position);
}

std::vector<Projectile> Game::GetProjectiles()
{
    return Get().m_projectiles;
}

Game& Game::Get()
{
    return s_instance;
}
