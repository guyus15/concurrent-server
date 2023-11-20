#include "application.h"

#include <common/utils/logging.h>

#include <steam/steamnetworkingsockets.h>

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

    SteamDatagramErrMsg err_msg;
    if (!GameNetworkingSockets_Init(nullptr, err_msg))
        SCX_CORE_ERROR("Failed to initialise GameNetworkingSockets: {0}.", err_msg);
}

void Application::Run() const
{
}

void Application::Dispose() const
{
}
