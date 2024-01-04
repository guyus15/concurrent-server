#include "common/networking/core.h"
#include "common/utils/logging.h"

#include <chrono>
#include <thread>

/**
 * \brief Specifies the time allocated in order to cleanup before connection
 * closure occurs.
 */
constexpr int CLEANUP_TIME = 500;

void InitialiseSteamDatagramConnectionSockets()
{
    SteamDatagramErrMsg error_msg;
    if (!GameNetworkingSockets_Init(nullptr, error_msg))
        SCX_CORE_ERROR("Failed to initialise GameNetworkingSockets: {0}.", error_msg);
}

void ShutdownSteamDatagramConnectionSockets()
{
    // Give connections time to finish up.
    std::this_thread::sleep_for(std::chrono::milliseconds(CLEANUP_TIME));
    GameNetworkingSockets_Kill();
}
