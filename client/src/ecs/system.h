#pragma once

class ISystem
{
public:
    ISystem() = default;

    ISystem(const ISystem&) = default;
    ISystem& operator=(const ISystem&) = default;

    ISystem(ISystem&&) noexcept = default;
    ISystem& operator=(ISystem&&) noexcept = default;

    virtual ~ISystem() = default;
    virtual void Update(double dt) = 0;
};
