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

    filter { "system:Windows" }
        removefiles { "src/networking/networking_linux.cpp" }
        defines { "SCX_WINDOWS_BUILD" }

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
