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
        "../thirdparty/game-networking/include"
    }

    links
    {
        "common"
    }

    filter { "system:Windows", "configurations:Debug" }
        links { "../thirdparty/game-networking/libs/Windows/Debug/GameNetworkingSockets.lib" }
        
        postbuildcommands
        {
            "{COPYFILE} ../thirdparty/game-networking/libs/Windows/Debug/GameNetworkingSockets.dll ../bin/%{cfg.buildcfg}",
            "{COPYFILE} ../thirdparty/game-networking/libs/Windows/Debug/libcrypto-3-x64.dll ../bin/%{cfg.buildcfg}",
            "{COPYFILE} ../thirdparty/game-networking/libs/Windows/Debug/libprotobuf.dll ../bin/%{cfg.buildcfg}"
        }

    filter { "system:Windows", "configurations:Release" }
        links { "../thirdparty/game-networking/libs/Windows/Release/GameNetworkingSockets.lib" }

        postbuildcommands
        {
            "{COPYFILE} ../thirdparty/game-networking/libs/Windows/Release/GameNetworkingSockets.dll ../bin/%{cfg.buildcfg}",
            "{COPYFILE} ../thirdparty/game-networking/libs/Windows/Release/libcrypto-3-x64.dll ../bin/%{cfg.buildcfg}",
            "{COPYFILE} ../thirdparty/game-networking/libs/Windows/Release/libprotobuf.dll ../bin/%{cfg.buildcfg}"
        }

    -- Add Linux specific post-build commands here.

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

