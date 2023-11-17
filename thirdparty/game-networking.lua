project "GameNetworkingSockets"
    kind "StaticLib"
    language "C++"

    targetdir "../libs/%{cfg.buildcfg}"
    objdir "../obj/%{cfg.buildcfg}"

    includedirs { "game-networking/include" }

    files
    {
        "game-networking/src/**.cpp",
        "game-networking/src/**.h"
    }

    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "On"

    filter { "configurations:Release or configurations:Dist" }
        runtime "Release"
        optimize "On"