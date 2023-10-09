workspace "concurrent-server-client"
    configurations{ "Debug", "Release", "Dist" }
    architecture "x86_64"
    startproject "server"

include "src/server.lua"
include "src/client.lua"