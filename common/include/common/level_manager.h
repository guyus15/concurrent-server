#pragma once

#include "common/level.h"

class LevelManager
{
public:
    LevelManager(const LevelManager&) = delete;
    LevelManager& operator=(const LevelManager&) = delete;

    LevelManager(LevelManager&&) noexcept = default;
    LevelManager& operator=(LevelManager&&) noexcept = default;

    static void Initialise();

    static void SetActive(int level_no);

    [[nodiscard]] static Level& GetActive();

private:
    int m_active_level;
    std::vector<Level> m_available_levels;

    LevelManager();
    ~LevelManager() = default;

    static LevelManager s_instance;
    static LevelManager& Get();
};
