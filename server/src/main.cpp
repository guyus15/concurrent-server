#include "application.h"

int main()
{
    ServerSettings server_settings{};
    server_settings.port = 27565;
    server_settings.tick_rate = 60;
    server_settings.max_no_clients = 4;

    Application application{ server_settings };
    application.Run();

    return 0;
}