#include "shader.h"

#include "utils/logging.h"

#include <fstream>

constexpr int INFO_LOG_SIZE = 500;

Shader::Shader()
    : m_id{}
{
}

Shader::Shader(std::string vertex_shader_path, std::string fragment_shader_path)
    : m_vertex_path{ std::move(vertex_shader_path) },
      m_fragment_path{ std::move(fragment_shader_path) },
      m_id{}
{
}

void Shader::Load()
{
    // Open vertex and fragment shaders.
    std::ifstream vertex_shader_file{ m_vertex_path, std::ios::in | std::ios::ate };
    if (!vertex_shader_file.is_open())
        SCX_CORE_ERROR("Failed to open vertex shader file at path '{0}'.", m_vertex_path);

    std::ifstream fragment_shader_file{ m_fragment_path, std::ios::in | std::ios::ate };
    if (!fragment_shader_file.is_open())
        SCX_CORE_ERROR("Failed to open fragment shader file at path '{0}'.", m_fragment_path);

    auto vertex_shader_length = vertex_shader_file.tellg();
    auto fragment_shader_length = fragment_shader_file.tellg();

    std::string vertex_shader_source(vertex_shader_length, '\0');
    std::string fragment_shader_source(fragment_shader_length, '\0');

    vertex_shader_file.seekg(0);
    fragment_shader_file.seekg(0);

    // Read the contents of the vertex and fragment shaders into their source strings
    // using the computed length.
    vertex_shader_file.read(vertex_shader_source.data(), vertex_shader_length);
    fragment_shader_file.read(fragment_shader_source.data(), fragment_shader_length);

    const char* vertex_shader_source_c = vertex_shader_source.c_str();
    const char* fragment_shader_source_c = fragment_shader_source.c_str();

    m_id = glCreateProgram();

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertex_shader, 1, &vertex_shader_source_c, nullptr);
    glShaderSource(fragment_shader, 1, &fragment_shader_source_c, nullptr);

    int success;
    char info_log[INFO_LOG_SIZE];

    // Compile vertex and fragment shaders.
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, INFO_LOG_SIZE, nullptr, info_log);
        SCX_CORE_WARN("Failed to compile vertex shader:\n{0}", info_log);
    }

    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, INFO_LOG_SIZE, nullptr, info_log);
        SCX_CORE_WARN("Failed to compile fragment shader:\n{0}", info_log);
    }

    glAttachShader(m_id, vertex_shader);
    glAttachShader(m_id, fragment_shader);

    // Link the shader program.
    glLinkProgram(m_id);
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_id, INFO_LOG_SIZE, nullptr, info_log);
        SCX_CORE_WARN("Failed to link shader program:\n{0}", info_log);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::Unload()
{
    glDeleteProgram(m_id);
}

void Shader::Use() const
{
    glUseProgram(m_id);
}

void Shader::SetBool(const std::string& name, const bool value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::SetInt(const std::string& name, const int value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, const float value) const
{
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}
