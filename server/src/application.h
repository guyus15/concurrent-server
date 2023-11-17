#pragma once

struct ServerSettings
{
    int port;
    int tick_rate;
    int max_no_clients;
};

class Application
{
public:
    explicit Application(ServerSettings settings);
    ~Application();

    Application(const Application &) = default;
    Application &operator=(const Application &) = default;

    Application(Application &&) noexcept = default;
    Application &operator=(Application &&) noexcept = default;

    void Run() const;

private:
    void Initialise();
    void Dispose() const;
};
