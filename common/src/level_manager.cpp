#include "common/assets/asset_manager.h"

#include "common/level_manager.h"

LevelManager LevelManager::s_instance{};

void LevelManager::Initialise()
{
    const Level test_level = AssetManager<Level>::LoadOrRetrieve("resources/levels/level1.xml");
    Get().m_available_levels.push_back(test_level);
}

void LevelManager::SetActive(const int level_no)
{
    Get().m_active_level = level_no;
}

Level& LevelManager::GetActive()
{
    return Get().m_available_levels[Get().m_active_level];
}

LevelManager& LevelManager::Get()
{
    return s_instance;
}

LevelManager::LevelManager()
    : m_active_level{ 0 }
{
}
