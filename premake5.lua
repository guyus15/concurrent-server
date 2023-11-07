workspace "concurrent-server-client"
    configurations { "Debug", "Release", "Dist" }
    architecture "x86_64"
    startproject "client"

    filter { "system:Windows" }
        defines { "SCX_WINDOWS_BUILD" }
    
    filter {}

    filter { "configurations:Debug or configurations:Release" }
        includedirs { "thirdparty/spdlog/include" }
        defines { "SCX_LOGGING" }

include "server/server.lua"
include "client/client.lua"
include "common/common.lua"
