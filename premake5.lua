workspace "concurrent-server-client"
    configurations { "Debug", "Release", "Dist" }
    architecture "x86_64"
    startproject "client"

include "server/server.lua"
include "client/client.lua"
include "common/common.lua"
