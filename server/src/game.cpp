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

    for (auto& projectile : Get().m_projectiles)
    {
        projectile.Update(dt);
    }
}

void Game::SpawnProjectile(const glm::vec2 position, const glm::vec2 direction)
{
    const Projectile projectile = Get().m_projectiles.emplace_back(position, direction);
    ProjectileUpdate(projectile);
}

std::vector<Projectile> Game::GetProjectiles()
{
    return Get().m_projectiles;
}

Game& Game::Get()
{
    return s_instance;
}
