workspace "concurrent-server-client"
    configurations{ "Debug", "Release", "Dist" }
    architecture "x86_64"
    startproject "client"

    filter { "configurations:Debug or configurations:Release" }
        includedirs { "thirdparty/spdlog/include" }
        defines { "SC_LOGGING" }

include "src/server.lua"
include "src/client.lua"