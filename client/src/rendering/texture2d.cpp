#include "texture2d.h"

#include <common/utils/logging.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture2d::Texture2d()
    : m_id{},
      m_width{},
      m_height{}
{
}

Texture2d::Texture2d(std::string path)
    : m_id{},
      m_width{},
      m_height{},
      m_path{ std::move(path) }
{
}

void Texture2d::Load()
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int no_components;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(m_path.c_str(), &m_width, &m_height, &no_components, 0);

    if (data)
    {
        // Determine the format of the texture data.
        GLenum format = 0;
        switch (no_components)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
        default:
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), m_width, m_height, 0, format, GL_UNSIGNED_BYTE,
                     data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    SCX_CORE_ERROR("Failed to load texture '{0}'.", m_path);

    // Unbind texture for future use.
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

void Texture2d::Unload()
{
    glDeleteTextures(1, &m_id);
}

void Texture2d::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}

GLuint Texture2d::GetId() const
{
    return m_id;
}
