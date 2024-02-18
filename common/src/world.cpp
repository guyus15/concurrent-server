#include "common/world.h"

#include "common/graphics/screen_manager.h"

glm::vec2 GetMousePositionToWorldSpace(const glm::vec2 mouse_position)
{
    const auto& [width, height, _] = ScreenManager::GetCurrentVideoMode();
    const float zoom_level_x = static_cast<float>(width) / static_cast<float>(WORLD_DIMENSIONS_X);
    const float zoom_level_y = static_cast<float>(height) / static_cast<float>(WORLD_DIMENSIONS_Y);

    return {
        mouse_position.x - static_cast<float>(WORLD_DIMENSIONS_X) / 2.0f * zoom_level_x,
        -mouse_position.y + static_cast<float>(WORLD_DIMENSIONS_Y) / 2.0f * zoom_level_y
    };
}
