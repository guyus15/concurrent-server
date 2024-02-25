project "server_tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir "../bin/tests"
    objdir "../obj/%{cfg.buildcfg}"

    files
    {
        "tests/**.cpp",
        "src/**.cpp"
    }

    removefiles
    {
        "src/main.cpp"
    }

    includedirs
    {
        "src",
        "../common/include",
        "../thirdparty/clove-unit",
        "../thirdparty/game-networking/include",
        "../thirdparty/glfw/include",
        "../thirdparty/glm"
    }

    links
    {
        "common"
    }
    
    filter { "system:Linux" }
        linkoptions { "-Wl,-rpath,\\$$ORIGIN" }

    filter {}

    filter { "system:Windows", "configurations:Debug" }
        links { "../thirdparty/game-networking/libs/Windows/Debug/GameNetworkingSockets.lib" }

    filter { "system:Windows", "configurations:Release or configurations:Dist" }
        links { "../thirdparty/game-networking/libs/Windows/Release/GameNetworkingSockets.lib" }

    filter { "system:Linux", "configurations:Debug"}
        libdirs { "../thirdparty/game-networking/libs/Linux/Debug"}
        links { "GameNetworkingSockets:shared" }

    filter { "system:Linux", "configurations:Release or configurations:Dist" }
        libdirs { "../thirdparty/game-networking/libs/Linux/Release"}
        links { "GameNetworkingSockets:shared" }

    filter {}

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