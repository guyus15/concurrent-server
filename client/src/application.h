#pragma once

#include <memory>

class Window;

class Application
{
public:
    Application();
    ~Application();

    void Initialise();
    void Run() const;
    void Dispose();

private:
    std::unique_ptr<Window> m_window;
};
