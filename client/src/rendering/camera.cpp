#include "camera.h"

#include <common/graphics/screen_manager.h>

#include <glm/ext/matrix_clip_space.hpp>

OrthographicCamera::OrthographicCamera(Window* window)
    : m_window{ window },
      m_projection{}
{
}

void OrthographicCamera::Initialise()
{
    CalculateMatrices();
}

void OrthographicCamera::CalculateMatrices()
{
    const float aspect_ratio = ScreenManager::GetCurrentAspectRatio();
    m_projection = glm::ortho(-aspect_ratio, aspect_ratio, -1.0f, 1.0f, -1.0f, 1.0f);
}

glm::mat4 OrthographicCamera::GetProjectionMatrix() const
{
    return m_projection;
}
