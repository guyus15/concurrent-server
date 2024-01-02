#pragma once

#include <common/graphics/window.h>

#include <glm/mat4x4.hpp>

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

    void CalculateMatrices();

    [[nodiscard]] glm::mat4 GetProjectionMatrix() const;

private:
    Window* m_window;
    glm::mat4 m_projection;
};
