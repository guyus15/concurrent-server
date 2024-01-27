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
    static void Update(double dt);
    static void SpawnPlayer(unsigned int id, const std::string& name, const glm::vec2& position);
    static void SpawnLocalPlayer(const std::string& name, const glm::vec2& position);

private:
    std::unique_ptr<Scene> m_scene;
    std::unordered_map<unsigned int, Entity> m_players;

    Game() = default;

    static Game s_instance;
    static Game& Get();
};