#include "camera.h"

#include <common/graphics/screen_manager.h>

#include <common/world.h>

#include <glm/ext/matrix_clip_space.hpp>

void OrthographicCamera::Initialise()
{
    CalculateMatrices();
}

void OrthographicCamera::CalculateMatrices()
{
    const VideoMode current_video_mode = ScreenManager::GetCurrentVideoMode();
    const auto width = static_cast<float>(current_video_mode.width);
    const auto height = static_cast<float>(current_video_mode.height);

    m_zoom_level = width / WORLD_DIMENSIONS_X;

    m_projection = glm::ortho((-width / 2) / m_zoom_level,
                              (width / 2) / m_zoom_level,
                              -(height / 2) / m_zoom_level,
                              (height / 2) / m_zoom_level,
                              -1.0f,
                              1.0f);
}

glm::mat4 OrthographicCamera::GetProjectionMatrix() const
{
    return m_projection;
}

float OrthographicCamera::GetZoomLevel() const
{
    return m_zoom_level;
}
