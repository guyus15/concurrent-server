#include "common/utils/logging.h"

#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Logging::s_core_logger;

void Logging::Initialise(const std::string& name)
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    s_core_logger = spdlog::stdout_color_mt(name);
    s_core_logger->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger>& Logging::GetCoreLogger()
{
    return s_core_logger;
}
