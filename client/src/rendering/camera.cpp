#include "camera.h"

#include <common/graphics/screen.h>

#include <GLFW/glfw3.h>

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
    int width, height;

    if (m_window == nullptr)
    {
        const Resolution resolution = GetCurrentResolution(glfwGetPrimaryMonitor());
        width = resolution.first;
        height = resolution.second;
    }
    else
    {
        const Dimensions dimensions = m_window->GetDimensions();
        width = dimensions.width;
        height = dimensions.height;
    }

    const float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    m_projection = glm::ortho(-aspect_ratio, aspect_ratio, -1.0f, 1.0f, -1.0f, 1.0f);
}

glm::mat4 OrthographicCamera::GetProjectionMatrix() const
{
    return m_projection;
}
