project "common"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    
    targetdir "../libs/%{cfg.buildcfg}"
    objdir "../obj/%{cfg.buildcfg}"

    files
    {
        "src/**.cpp",
        "src/**.h"
    }

    includedirs
    {
        "include",
        "../thirdparty/game-networking/include"
    }

    filter { "system:Windows", "configurations:Debug" }
        links { "../thirdparty/game-networking/libs/Windows/Debug/GameNetworkingSockets.lib" }
    
    filter { "system:Windows", "configurations:Release" }
        links { "../thirdparty/game-networking/libs/Windows/Release/GameNetworkingSockets.lib" }

    -- Add Linux specific post-build commands here.

    filter {}

    filter { "system:Windows" }
        removefiles { "src/networking/networking_linux.cpp" }

    filter { "system:Linux" }
        removefiles { "src/networking/networking_windows.cpp" }

    filter {}

    filter { "configurations:Debug or configurations:Release" }
        includedirs { "../thirdparty/spdlog/include" }

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
