#pragma once

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
};
