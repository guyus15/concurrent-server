workspace "concurrent-server-client"
    configurations{ "Debug", "Release", "Dist" }
    architecture "x86_64"
    startproject "client"

    includedirs
    {
        "thirdparty/include/glad"
    }
    
    links
    {
        "GLAD"
    }

include "thirdparty/glad.lua"

include "src/server.lua"
include "src/client.lua"