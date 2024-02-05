#include "common/level.h"

#include "common/utils/logging.h"

#include "common/utils/assertion.h"

#include <tinyxml2.h>

#include <algorithm>

static std::vector<std::string> SplitString(const std::string& string, const char delimiter);

Level::Level(std::string path)
    : m_path{ std::move(path) }
{
}

void Level::Load()
{
    tinyxml2::XMLDocument level_file{};

    tinyxml2::XMLError status = level_file.LoadFile(m_path.c_str());
    if (status != tinyxml2::XML_SUCCESS)
    {
        SCX_CORE_ERROR("Failed to load level at the path {0}.", m_path);
        return;
    }

    tinyxml2::XMLElement* root_element = level_file.FirstChildElement("Level");

    if (!root_element)
    {
        SCX_CORE_ERROR("Expected level root node to begin with 'Level' element.");
        return;
    }

    if (!root_element->Attribute("Name"))
    {
        SCX_CORE_ERROR("Expected 'Level' element to have a 'Name' attribute.");
        return;
    }

    m_name = root_element->Attribute("Name");

    tinyxml2::XMLElement* first_content_element = root_element->FirstChildElement();

    if (!first_content_element)
    {
        SCX_CORE_WARN("No content found in the level file '{0}'.", m_path);
        return;
    }

    tinyxml2::XMLElement* current_content_element = first_content_element;

    // Continue to read the level file while contents still exists.
    bool content_exists = true;
    while (content_exists)
    {
        content_exists = false;

        LevelContent content{};

        std::string element_name = current_content_element->Name();

        // Determine the type of level content.
        if (element_name == std::string{ "Platform" })
            content.type = LevelContent::Type::Platform;

        else if (element_name == std::string{ "Wall" })
            content.type = LevelContent::Type::Wall;

        else if (element_name == std::string{ "PlayerSpawnPoint" })
            content.type = LevelContent::Type::PlayerSpawnPoint;

        // Attempt to read the level content's position attribute.
        if (!current_content_element->Attribute("Position"))
        {
            SCX_CORE_ERROR("Expected '{0}' element to have a 'Position' attribute.", element_name);
            return;
        }

        std::vector<std::string> split_position = SplitString(current_content_element->Attribute("Position"), ',');
        float position_x = std::stof(split_position[0]);
        float position_y = std::stof(split_position[1]);

        content.position = { position_x, position_y };

        // Attempt to read the level's scale attribute.
        if (!current_content_element->Attribute("Scale"))
        {
            SCX_CORE_ERROR("Expected '{0}' element to have a 'Scale' attribute.", element_name);
            return;
        }

        std::vector<std::string> split_scale = SplitString(current_content_element->Attribute("Scale"), ',');
        float scale_x = std::stof(split_scale[0]);
        float scale_y = std::stof(split_scale[1]);

        content.scale = { scale_x, scale_y };

        m_contents.push_back(content);
    }

    SCX_CORE_INFO("Successfully loaded level '{0}' at '{1}'.", m_name, m_path);
}

void Level::Unload()
{
}

std::vector<LevelContent> Level::GetRenderableContent() const
{
    std::vector<LevelContent> renderable{};

    for (const auto level_content : m_contents)
    {
        if (level_content.type == LevelContent::Type::PlayerSpawnPoint)
            continue;

        renderable.push_back(level_content);
    }

    return renderable;
}

std::vector<LevelContent> Level::GetByType(const LevelContent::Type type) const
{
    std::vector<LevelContent> type_contents{};

    for (const auto content : m_contents)
    {
        if (content.type == type)
            type_contents.push_back(content);
    }

    return type_contents;
}

std::vector<std::string> SplitString(const std::string& string, const char delimiter)
{
    std::stringstream stream{ string };
    std::string split_segment;
    std::vector<std::string> segment_list;

    while (std::getline(stream, split_segment, delimiter))
        segment_list.push_back(split_segment);

    return segment_list;
}
