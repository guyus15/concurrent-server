#pragma once

#include "asset.h"

#include <glad/glad.h>

#include <string>

/**
 * \brief Provides an abstraction for an OpenGL 2D texture.
 */
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

    /**
     * \brief Loads the texture.
     */
    void Load() override;

    /**
     * \brief Unloads the texture.
     */
    void Unload() override;

    /**
     * \brief Binds the texture to be used.
     */
    void Bind() const;

    /**
     * \brief Gets the ID of the texture.
     * \return The texture's ID.
     */
    [[nodiscard]] GLuint GetId() const;

private:
    GLuint m_id;
    GLsizei m_width, m_height;
    std::string m_path;
};
