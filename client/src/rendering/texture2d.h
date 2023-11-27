#pragma once

#include "asset.h"

#include <glad/glad.h>

#include <string>

class Texture2d final : public Asset
{
public:
    Texture2d();
    explicit Texture2d(std::string path);
    ~Texture2d() override = default;

    Texture2d(const Texture2d&) = default;
    Texture2d& operator=(const Texture2d&) = default;

    Texture2d(Texture2d&&) noexcept = default;
    Texture2d& operator=(Texture2d&&) noexcept = default;

    void Load() override;
    void Unload() override;

    void Bind() const;

    [[nodiscard]] GLuint GetId() const;

private:
    GLuint m_id;
    GLsizei m_width, m_height;
    std::string m_path;
};
