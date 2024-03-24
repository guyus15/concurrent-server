#pragma once

#include <string>
#include <chrono>

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
    int width, height, refresh_rate;
};

struct OnConnectEvent final : GameEvent
{
    std::string username;
    std::string ip;
    int port{};
};

struct OnConnectStatusEvent final : GameEvent
{
    bool success;
    std::string info;
};

struct OnScreenSettingsVisibleEvent final : GameEvent
{
};

struct OnChatVisibleEvent final : GameEvent
{
    bool visible;
};

struct OnChatSendEvent final : GameEvent
{
    std::string message;
};

struct OnChatReceiveEvent final : GameEvent
{
    std::chrono::time_point<std::chrono::system_clock> timestamp;
    std::string author;
    std::string message;
};

struct OnLocalPlayerDeathEvent final : GameEvent
{
};

struct OnLocalPlayerRespawnEvent final : GameEvent
{
};
