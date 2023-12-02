#pragma once

#include <string>

const std::string DEFAULT_TITLE = "Untitled";

class UserInterface
{
public:
    UserInterface() = default;
    virtual ~UserInterface() = default;

    UserInterface(const UserInterface&) = default;
    UserInterface& operator=(const UserInterface&) = default;

    UserInterface(UserInterface&&) noexcept = default;
    UserInterface& operator=(UserInterface&&) noexcept = default;

    virtual void Initialise()
    {
    }

    virtual void Update(double delta_time) = 0;

    virtual void Dispose()
    {
    }

protected:
    std::string m_title;
    bool m_show;
};
