#include "common/world.h"

#include "common/graphics/screen_manager.h"

glm::vec2 GetMousePositionToWorldSpace(const glm::vec2 mouse_position)
{
    const auto& [width, height, _] = ScreenManager::GetCurrentVideoMode();

    return {
        mouse_position.x / static_cast<float>(width) * WORLD_DIMENSIONS_X - WORLD_DIMENSIONS_X / 2.0f,
        -(mouse_position.y / static_cast<float>(height) * WORLD_DIMENSIONS_Y - WORLD_DIMENSIONS_Y / 2.0f)
    };
}
