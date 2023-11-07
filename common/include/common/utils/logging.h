#pragma once

#ifdef SCX_LOGGING
#include <spdlog/spdlog.h>

#include <memory>

class Logging
{
public:
    /**
     * \brief Initialises application logging.
     */
    static void Initialise();

    /**
     * \brief Gets a reference to the application's core logger.
     * \return A reference to the core's logger.
     */
    static std::shared_ptr<spdlog::logger>& GetCoreLogger();

private:
    static std::shared_ptr<spdlog::logger> s_core_logger;
};

#define SCX_CORE_TRACE(...) Logging::GetCoreLogger()->trace(__VA_ARGS__)
#define SCX_CORE_INFO(...) Logging::GetCoreLogger()->info(__VA_ARGS__)
#define SCX_CORE_WARN(...) Logging::GetCoreLogger()->warn(__VA_ARGS__)
#define SCX_CORE_ERROR(...) Logging::GetCoreLogger()->error(__VA_ARGS__)
#define SCX_CORE_CRITICAL(...) Logging::GetCoreLogger()->critical(__VA_ARGS__)
#else
#define SCX_CORE_TRACE(...)
#define SCX_CORE_INFO(...)
#define SCX_CORE_WARN(...)
#define SCX_CORE_ERROR(...)
#define SCX_CORE_CRITICAL(...)

#endif // SCX_LOGGING
