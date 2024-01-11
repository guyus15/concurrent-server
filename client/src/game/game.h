#pragma once

#include <vector>

class Game
{
public:
    Game() = default;
    ~Game() = default;

private:
    std::vector<Player> m_players;
};