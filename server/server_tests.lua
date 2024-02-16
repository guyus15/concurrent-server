project "server_tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir "../bin/tests"
    objdir "../obj/%{cfg.buildcfg}"

    files
    {
        "tests/**.cpp",
        "src/player.cpp",
        "src/asset_manager.cpp"
    }

    includedirs
    {
        "src",
        "../common/include",
        "../thirdparty/clove-unit",
        "../thirdparty/glm"
    }

    links
    {
        "common"
    }
    
    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "On"

    filter { "configurations:Release" }
        runtime "Release"
        optimize "On"

    filter { "configurations:Dist" }
        runtime "Release"
        optimize "On"

include "server.lua"
include "../common/common.lua"