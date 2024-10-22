project "GLFW"
    location "GLFW"
    kind "StaticLib"  -- Static library
    language "C"

    targetdir ("../../Binaries/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
    objdir ("../../Binaries/Intermediate/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")

    files
    {
        "../GLFW/src/**.c",  -- Reference GLFW submodule files
        "../GLFW/include/GLFW/**.h"  -- Reference GLFW headers from submodule
    }

    includedirs
    {
        "../GLFW/include"  -- Include directory from submodule
    }

    filter "system:windows"
        buildoptions { "-std=c11", "-lgdi32" }
        systemversion "latest"
        staticruntime "On"

        defines
        {
            "_GLFW_WIN32",
            "GLFW_STATIC"  -- If using GLFW as a static library
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"
