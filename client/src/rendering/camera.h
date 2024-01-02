#pragma once

#include <common/graphics/window.h>

#include <glm/mat4x4.hpp>

/**
 * \brief Calculates the necessary projection matrix to display an
 * the world with an orthogonal perspective.
 */
class OrthographicCamera
{
public:
    explicit OrthographicCamera(Window* window);
    ~OrthographicCamera() = default;

    OrthographicCamera(const OrthographicCamera&) = default;
    OrthographicCamera& operator=(const OrthographicCamera&) = default;

    OrthographicCamera(OrthographicCamera&&) noexcept = default;
    OrthographicCamera& operator=(OrthographicCamera&&) noexcept = default;

    void Initialise();

    /**
     * \brief Calculates the projection matrix based on the current screen dimensions.
     */
    void CalculateMatrices();

    /**
     * \brief Gets the camera's projection matrix.
     * \return The camera's projection matrix.
     */
    [[nodiscard]] glm::mat4 GetProjectionMatrix() const;

private:
    Window* m_window;
    glm::mat4 m_projection;
};
