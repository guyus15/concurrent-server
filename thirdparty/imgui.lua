project "DearImGui"
    kind "StaticLib"
    language "C++"

    targetdir "../libs/%{cfg.buildcfg}"
    objdir "../obj/%{cfg.buildcfg}"

    includedirs { "imgui" }

    files
    {
        "imgui/imgui.cpp",
        "imgui/imgui_demo.cpp",
        "imgui/imgui_draw.cpp",
        "imgui/imgui_tables.cpp",
        "imgui/imgui_widgets.cpp",
        "imgui/backends/imgui_impl_opengl3.cpp",
        "imgui/backends/imgui_impl_glfw.cpp"
    }

    includedirs { "glfw/include" }

    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "On"

    filter { "configurations:Release or configurations:Dist" }
        runtime "Release"
        optimize "On"