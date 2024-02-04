workspace "concurrent-server-client"
    configurations { "Debug", "Release", "Dist" }
    architecture "x86_64"
    startproject "client"

    defines { "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING" }

    filter { "configurations:Debug or configurations:Release" }
        includedirs { "thirdparty/spdlog/include" }
        defines { "SCX_LOGGING", "SCX_ASSERTION" }

include "server/server.lua"
include "client/client.lua"
include "common/common.lua"

include "common/common_tests.lua"
include "server/server_tests.lua"
