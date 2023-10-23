#pragma once

#include "asset.h"

#include <glad/glad.h>

#include <string>

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

    void Load() override;
    void Unload() override;

    void Use() const;

    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;

private:
    std::string m_vertex_path;
    std::string m_fragment_path;

    GLuint m_id;
};
