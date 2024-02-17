project "common"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    
    targetdir "../libs/%{cfg.buildcfg}"
    objdir "../obj/%{cfg.buildcfg}"

    prebuildcommands
    {
        "{MKDIR} ../bin/%{cfg.buildcfg}"
    }

    files
    {
        "src/**.cpp",
        "src/**.h",
        "../thirdparty/tinyxml2/tinyxml2.cpp"
    }

    includedirs
    {
        "include",
        "../thirdparty/game-networking/include",
        "../thirdparty/glfw/include",
        "../thirdparty/glm",
        "../thirdparty/tinyxml2"
    }

    links { "GLFW" }

    filter { "system:Windows" }
        removefiles { "src/networking/networking_linux.cpp" }

    filter { "system:Linux" }
        removefiles { "src/networking/networking_windows.cpp" }

    filter {}

    filter { "system:Windows", "configurations:Debug" }
        links { "../thirdparty/game-networking/libs/Windows/Debug/GameNetworkingSockets.lib" }

        postbuildcommands
        {
            "{COPYFILE} ../thirdparty/game-networking/libs/Windows/Debug/GameNetworkingSockets.dll ../bin/%{cfg.buildcfg}",
            "{COPYFILE} ../thirdparty/game-networking/libs/Windows/Debug/libcrypto-3-x64.dll ../bin/%{cfg.buildcfg}",
            "{COPYFILE} ../thirdparty/game-networking/libs/Windows/Debug/libprotobufd.dll ../bin/%{cfg.buildcfg}"
        }

    filter { "system:Windows", "configurations:Release or configurations:Dist" }
        links { "../thirdparty/game-networking/libs/Windows/Release/GameNetworkingSockets.lib" }

        postbuildcommands
        {
            "{COPYFILE} ../thirdparty/game-networking/libs/Windows/Release/GameNetworkingSockets.dll ../bin/%{cfg.buildcfg}",
            "{COPYFILE} ../thirdparty/game-networking/libs/Windows/Release/libcrypto-3-x64.dll ../bin/%{cfg.buildcfg}",
            "{COPYFILE} ../thirdparty/game-networking/libs/Windows/Release/libprotobuf.dll ../bin/%{cfg.buildcfg}"
        }

    filter { "system:Linux", "configurations:Debug"}
        libdirs { "../thirdparty/game-networking/libs/Linux/Debug"}
        links { "GameNetworkingSockets:shared" }

        postbuildcommands
        {
            "{COPYFILE} ../thirdparty/game-networking/libs/Linux/Debug/libGameNetworkingSockets.so ../bin/%{cfg.buildcfg}"
        }

    filter { "system:Linux", "configurations:Release or configurations:Dist" }
        libdirs { "../thirdparty/game-networking/libs/Linux/Release"}
        links { "GameNetworkingSockets:shared" }

        postbuildcommands
        {
            "{COPYFILE} ../thirdparty/game-networking/libs/Linux/Release/libGameNetworkingSockets.so ../bin/%{cfg.buildcfg}"
        }

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
        removefiles{ "src/utils/logging.cpp" }
        runtime "Release"
        optimize "On"

include "../thirdparty/glfw.lua"
