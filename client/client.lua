project "client"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir "../bin/%{cfg.buildcfg}"
    objdir "../obj/%{cfg.buildcfg}"

    filter { "system:Windows"}
        postbuildcommands
        {
            "{MKDIR} ../bin/%{cfg.buildcfg}/resources",
            "{COPYDIR} resources ../bin/%{cfg.buildcfg}/resources"
        }

    filter { "system:Linux" }
        postbuildcommands
        {
            "{COPYDIR} resources ../bin/%{cfg.buildcfg}"
        }

    filter {}

    files
    {
        "src/**.cpp",
        "src/**.h"
    }

    includedirs
    {
        "../include",
        "src",
        "../thirdparty/glad/include",
        "../thirdparty/glfw/include",
        "../thirdparty/stb/include",
        "../thirdparty/glm"
    }
    
    links
    {
        "GLAD",
        "GLFW"
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
include "thirdparty/glfw.lua"
