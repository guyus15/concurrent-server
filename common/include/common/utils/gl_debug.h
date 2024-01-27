#pragma once

#include "logging.h"

#include <glad/glad.h>

inline GLenum GlCheckError(const char* file, const int line)
{
    GLenum error_code;
    while ((error_code = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (error_code)
        {
        case GL_INVALID_ENUM: error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE: error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION: error = "INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW: error = "STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW: error = "STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }

        SCX_CORE_ERROR("GL Error - {0} ({1}): {2}", file, line, error);
    }

    return error_code;
}

#define GL_CHECK_ERROR() GlCheckError(__FILE__, __LINE__)
