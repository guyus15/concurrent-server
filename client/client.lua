project "client"
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
        "../include",
        "./",
        "../thirdparty/glad/include",
        "../thirdparty/glfw/include"
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