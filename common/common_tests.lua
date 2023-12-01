project "common_tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir "../bin/tests"
    objdir "../obj/%{cfg.buildcfg}"

    files
    {
        "tests/**.cpp"
    }

    includedirs
    {
        "include",
        "../thirdparty/clove-unit",
        "../thirdparty/spdlog/include",
        "../thirdparty/game-networking/include"
    }

    links "common"

    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "On"

    filter { "configurations:Release" }
        runtime "Release"
        optimize "On"

    filter { "configurations:Dist" }
        runtime "Release"
        optimize "On"

include "common"
