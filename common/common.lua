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
        "include"
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
