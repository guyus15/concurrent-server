#pragma once

#include <memory>

class Window;

class Application
{
public:
    Application();
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    Application(Application&&) noexcept = default;
    Application& operator=(Application&&) noexcept = default;

    void Run() const;

private:
    void Initialise();
    void Dispose();

    std::unique_ptr<Window> m_window;
};
