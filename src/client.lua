project "client"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir "../bin/%{cfg.buildcfg}"
    objdir "../obj/%{cfg.buildcfg}"

    files
    {
        "client/**.cpp",
        "client/**.h"
    }

    includedirs
    {
        "../include",
        "./",
        "thirdparty/include/glad"
    }
    
    links
    {
        "GLAD"
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

include "thirdparty/glad.lua"