project "ImGui"
    kind "StaticLib"  -- Static library
    language "C++"

    targetdir ("../../Binaries/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
    objdir ("../../Binaries/Intermediate/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")

    files
    {
        "../imgui/imconfig.h",
        "../imgui/imgui.h",
        "../imgui/imgui.cpp",
        "../imgui/imgui_draw.cpp",
        "../imgui/imgui_internal.h",
        "../imgui/imgui_tables.cpp",
        "../imgui/imgui_widgets.cpp",
        "../imgui/imstb_rectpack.h",
        "../imgui/imstb_textedit.h",
        "../imgui/imstb_truetype.h",
        "../imgui/imgui_demo.cpp",
    }

    includedirs
    {
        "../imgui/backends/",  -- Include directory
        "../imgui/"  -- Include directory
    }

    filter "system:windows"
        systemversion "latest"
        cppdialect "C++20"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"
