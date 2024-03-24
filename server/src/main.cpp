#include "server.h"

#include <iostream>
#include <string>

bool FindCommandOption(char* begin[], char* end[], const std::string& option, std::string& value)
{
    auto it = std::find(begin, end, option);
    if (it != end && ++it != end)
        value = *it;
    else
        return false;

    return true;
}

bool ParseArguments(const int argc, char* argv[], ServerSettings& settings)
{
    std::string port_string;
    std::string tick_rate_string;

    if (!FindCommandOption(argv + 1, argv + argc, "-port", port_string))
        return false;
    if (!FindCommandOption(argv + 1, argv + argc, "-tick-rate", tick_rate_string))
        return false;

    settings.port = static_cast<uint16_t>(std::stoi(port_string));
    settings.tick_rate = std::stoi(tick_rate_string);

    return true;
}

int main(const int argc, char* argv[])
{
    ServerSettings server_settings{};

    if (!ParseArguments(argc, argv, server_settings))
    {
        std::cerr <<
            "Invalid command line arguments. Usage: ./server -port [port] -tick-rate [tick rate]\n";

        // If invalid command line arguments have been passed to the program, just use default settings.
        server_settings.port = 27565;
        server_settings.tick_rate = 60;
    }

    Server server{ server_settings };
    server.Run();

    return 0;
}
