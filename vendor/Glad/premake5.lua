project "Glad"
    kind "StaticLib"  -- Static library
    language "C"

    targetdir ("../../Binaries/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
    objdir ("../../Binaries/Intermediate/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")

    files
    {
        "src/glad.c",  -- Reference GLAD src files
        "include/KHR/khrplatform.h",  -- Reference khrplatform headers from submodule
        "include/glad/glad.h"  -- Reference glad.h header file
    }

    includedirs
    {
        "include/"  -- Include directory
    }

    filter "system:windows"
        buildoptions { "-std=c11", "-lgdi32" }
        systemversion "latest"
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
