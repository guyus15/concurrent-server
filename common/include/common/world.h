#pragma once

#include <glm/vec2.hpp>

constexpr float WORLD_DIMENSIONS_X = 640.0f,
                WORLD_DIMENSIONS_Y = 480.0f;

/**
 * \brief Gets the mouse position in world space.
 * \param mouse_position The current position of the mouse.
 * \return The mouse's position in world space.
 */
glm::vec2 GetMousePositionToWorldSpace(glm::vec2 mouse_position);