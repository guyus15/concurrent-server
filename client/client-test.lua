project "client-test"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir "../bin/%{cfg.buildcfg}"
    objdir "../obj/%{cfg.buildcfg}"

    files
    {
        "src/**.cpp",
        "src/**.h",
        "tests/**.cpp"
    }

    removefiles
    {
        "src/main.cpp" -- Avoid multiple definitions of main
    }

    includedirs
    {
        "../include",
        "src",
        "../thirdparty/glad/include",
        "../thirdparty/glfw/include",
        "../thirdparty/glm",
        "../thirdparty/clove-unit"
    }

    links
    {
        "GLAD",
        "GLFW"
    }

include "thirdparty/glad.lua"
include "thirdparty/glfw.lua"