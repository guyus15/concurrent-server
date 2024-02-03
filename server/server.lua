project "server"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir "../bin/%{cfg.buildcfg}"
    objdir "../obj/%{cfg.buildcfg}"

    files
    {
        "src/**.cpp",
        "src/**.h"
    }

    includedirs
    {
        "../common/include",
        "../thirdparty/game-networking/include",
        "../thirdparty/glfw/include",
        "../thirdparty/glm"
    }

    links
    {
        "common",
        "GLFW",
        "GLAD"
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

include "common/common.lua"
include "../thirdparty/glad.lua"
include "../thirdparty/glfw.lua"
