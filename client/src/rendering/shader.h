#pragma once

#include "asset.h"

#include <glad/glad.h>

#include <glm/mat4x4.hpp>

#include <string>

/**
 * \brief Provides an abstraction for an OpenGL shader.
 */
class Shader final : public Asset
{
public:
    Shader();
    Shader(std::string vertex_shader_path, std::string fragment_shader_path);
    ~Shader() override = default;

    Shader(const Shader&) = default;
    Shader& operator=(const Shader&) = default;

    Shader(Shader&&) noexcept = default;
    Shader& operator=(Shader&&) noexcept = default;

    /**
     * \brief Loads the shader.
     */
    void Load() override;

    /**
     * \brief Unloads the shader.
     */
    void Unload() override;

    /**
     * \brief Sets the shader to be used.
     */
    void Use() const;

    /**
     * \brief Sets a uniform bool value in the shader.
     * \param name The name of the uniform.
     * \param value The value of the uniform.
     */
    void SetBool(const std::string& name, bool value) const;

    /**
     * \brief Sets a uniform integer value in the shader.
     * \param name The name of the uniform.
     * \param value The value of the uniform.
     */
    void SetInt(const std::string& name, int value) const;

    /**
     * \brief Sets a uniform float value in the shader.
     * \param name The name of the uniform.
     * \param value The value of the uniform.
     */
    void SetFloat(const std::string& name, float value) const;

    /**
     * \brief Sets a uniform vec3 value in the shader.
     * \param name The name of the uniform.
     * \param value The value of the uniform.
     */
    void SetVec3(const std::string& name, glm::vec3 value) const;

    /**
     * \brief Sets a uniform mat4x4 value in the shader.
     * \param name The name of the uniform.
     * \param value The value of the uniform.
     */
    void SetMat4x4(const std::string& name, glm::mat4 value) const;

private:
    std::string m_vertex_path;
    std::string m_fragment_path;

    GLuint m_id;
};
