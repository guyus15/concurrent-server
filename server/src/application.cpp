#include "application.h"

#include <common/utils/logging.h>

Application::Application(ServerSettings settings)
{
    Initialise();
}

Application::~Application()
{
    Dispose();
}

void Application::Initialise()
{
    Logging::Initialise();
}

void Application::Run() const
{
}

void Application::Dispose() const
{
}
