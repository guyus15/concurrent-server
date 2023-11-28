#pragma once

#include "common/graphics/window.h"

#include <memory>

class IApplication
{
public:
    IApplication() = default;

    IApplication(const IApplication&) = default;
    IApplication& operator=(const IApplication&) = default;

    IApplication(IApplication&&) noexcept = default;
    IApplication& operator=(IApplication&&) noexcept = default;

    virtual ~IApplication() = default;

    virtual void Initialise() = 0;
    virtual void Run() = 0;
    virtual void Dispose() = 0;

protected:
    std::unique_ptr<Window> m_window;
};
