#pragma once

#include <chrono>

class Clock
{
public:
    Clock();
    ~Clock() = default;

    Clock(const Clock&) = default;
    Clock& operator=(const Clock&) = default;

    Clock(Clock&&) noexcept = default;
    Clock& operator=(Clock&&) noexcept = default;

    bool HasTimeElapsed(double seconds);

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;
};
