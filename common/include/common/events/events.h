#pragma once

#include <string>

typedef size_t EventId;

struct GameEvent
{
    GameEvent() = default;

    GameEvent(const GameEvent&) = default;
    GameEvent& operator=(const GameEvent&) = default;

    GameEvent(GameEvent&&) noexcept = default;
    GameEvent& operator=(GameEvent&&) noexcept = default;

    virtual ~GameEvent() = default;
};

// Events
struct WindowResizeEvent final : GameEvent
{
    bool fullscreen;
    int width, height;
};

struct FrameBufferResizeEvent final : GameEvent
{
    int width, height;
};

struct OnConnectEvent final : GameEvent
{
    std::string username;
    std::string ip;
    int port;
};

struct Events
{
    inline static const OnConnectEvent on_connect_event{};
};
