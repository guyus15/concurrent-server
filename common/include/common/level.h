#pragma once

#include "common/assets/asset.h"

#include <glm/vec2.hpp>

#include <string>
#include <vector>
#include <unordered_map>

struct LevelContent
{
    enum class Type
    {
        Platform,
        Wall,
        PlayerSpawnPoint
    } type;

    glm::vec2 position;
    glm::vec2 scale;
};

struct LevelContentCache
{
    std::vector<LevelContent> content;
    bool cached = false;
};

class Level final : public Asset
{
public:
    Level() = default;
    explicit Level(std::string path);
    ~Level() override = default;

    Level(const Level&) = default;
    Level& operator=(const Level&) = default;

    Level(Level&&) noexcept = default;
    Level& operator=(Level&&) noexcept = default;

    /**
     * \brief Loads the level.
     */
    void Load() override;

    /**
     * \brief Unloads the level.
     */
    void Unload() override;

    /**
     * \brief Gets the renderable contents of the level.
     * \return The renderable contents of the level.
     */
    [[nodiscard]] std::vector<LevelContent>& GetRenderableContent();

    /**
     * \brief Gets the contents of the given type which exist in the level.
     * \return The level's contents of a particular type.
     */
    [[nodiscard]] std::vector<LevelContent>& GetByType(LevelContent::Type type);

private:
    std::string m_path;
    std::string m_name;
    std::vector<LevelContent> m_contents;
    std::vector<LevelContent> m_renderable;
    bool m_renderable_checked;
    std::unordered_map<LevelContent::Type, LevelContentCache> m_type_contents;
};

