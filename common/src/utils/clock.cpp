#include "common/utils/clock.h"

Clock::Clock()
    : m_start_time{ std::chrono::high_resolution_clock::now() }
{
}

bool Clock::HasTimeElapsed(const double seconds)
{
    const auto current_time = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed_seconds = current_time - m_start_time;

    if (elapsed_seconds.count() >= seconds)
    {
        m_start_time = current_time;
        return true;
    }

    return false;
}
