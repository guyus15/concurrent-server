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
        "src",
        "../common/include",
        "../thirdparty/glad/include",
        "../thirdparty/glfw/include",
        "../thirdparty/stb/include",
        "../thirdparty/glm",
        "../thirdparty/game-networking/include"
    }
    
    links
    {
        "common",
        "GLAD",
        "GLFW"
    }

    filter { "system:Windows", "configurations:Debug" }
        links { "../thirdparty/game-networking/libs/Windows/Debug/GameNetworkingSockets.lib" }
        
        postbuildcommands
        {
            "{COPYFILE} ../thirdparty/game-networking/libs/Windows/Debug/GameNetworkingSockets.dll ../bin/%{cfg.buildcfg}",
            "{COPYFILE} ../thirdparty/game-networking/libs/Windows/Debug/libcrypto-3-x64.dll ../bin/%{cfg.buildcfg}",
            "{COPYFILE} ../thirdparty/game-networking/libs/Windows/Debug/libprotobufd.dll ../bin/%{cfg.buildcfg}"
        }

    filter { "system:Windows", "configurations:Release" }
        links { "../thirdparty/game-networking/libs/Windows/Release/GameNetworkingSockets.lib" }

        postbuildcommands
        {
            "{COPYFILE} ../thirdparty/game-networking/libs/Windows/Release/GameNetworkingSockets.dll ../bin/%{cfg.buildcfg}",
            "{COPYFILE} ../thirdparty/game-networking/libs/Windows/Release/libcrypto-3-x64.dll ../bin/%{cfg.buildcfg}",
            "{COPYFILE} ../thirdparty/game-networking/libs/Windows/Release/libprotobuf.dll ../bin/%{cfg.buildcfg}"
        }

    filter { "system:Linux" }
        linkoptions { "-Wl,-rpath,\\$$ORIGIN" }

    filter { "system:Linux", "configurations:Debug"}
        libdirs { "../thirdparty/game-networking/libs/Linux/Debug"}
        links { "GameNetworkingSockets:shared" }

    filter { "system:Linux", "configurations:Release or configurations:Dist" }
        libdirs { "../thirdparty/game-networking/libs/Linux/Release"}
        links { "GameNetworkingSockets:shared" }

    filter {}

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

    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "On"
    
    filter { "configurations:Release" }
        runtime "Release"
        optimize "On"

    filter { "configurations:Dist" }
        runtime "Release"
        optimize "On"

include "common/common.lua"
include "thirdparty/glad.lua"
include "thirdparty/glfw.lua"
