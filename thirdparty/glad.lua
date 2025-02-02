project "GLAD"
    kind "StaticLib"
    language "C"

    targetdir "../libs/%{cfg.buildcfg}"
    objdir "../obj/%{cfg.buildcfg}"

    includedirs { "glad/include" }
    files { "glad/src/glad.c" }

    filter { "system:Linux" }
        pic "On"
        systemversion "latest"
        staticruntime "On"

        defines { "_GLAD_X11" }

    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "On"

    filter { "configurations:Release or configurations:Dist" }
        runtime "Release"
        optimize "On"