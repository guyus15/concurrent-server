#pragma once

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
struct OnConnectEvent final : GameEvent
{
};

struct Events
{
    inline static const OnConnectEvent on_connect_event{};
};
