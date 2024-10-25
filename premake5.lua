workspace "Graphite"
	architecture "x64"
	startproject "SandBox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

-- Define the output directory structure using tokens
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include GLFW's Premake file
include "vendor/Glad"
include "vendor/GLFW_premake"
include "vendor/imgui_premake"

project "Graphite"
	location "Graphite"
	kind "SharedLib"
	language "C++"

	-- Target and object directories with correct separators
	targetdir ("Binaries/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
	objdir ("Binaries/Intermediate/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")

	-- Add precompiled header files
	pchheader "gfpch.h"
	pchsource "Graphite/src/gfpch.cpp"

	-- Add source files
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	-- Correct includedirs syntax, removing potential curly brace errors
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/glm",  -- Add glm directory
		"vendor/GLFW/include",  -- Add GLFW include directory
		"vendor/Glad/include",  -- Add Glad include directory
		"vendor/imgui",  -- Add imgui  directory
		"vendor/imgui/backends",  -- Add imgui backends directory
	}

	links
    {
        "Glad",  -- Link to GLAD
        "GLFW",  -- Link to GLFW
        "imgui",  -- Link to GLFW
        "opengl32.lib"  -- Link to OpenGL (for Windows)
    }

	-- Platform-specific settings
	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"GF_PLATFORM_WINDOWS",
			"GF_BUILD_DLL",
		    "GLFW_INCLUDE_NONE"
		}
		
		-- Use postbuild command to copy DLL to the Sandbox directory
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../Binaries/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/SandBox")
		}

	-- Configuration-specific settings
	filter "configurations:Debug"
		defines "GF_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "GF_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "GF_DIST"
		optimize "On"

project "SandBox"
	location "SandBox"
	kind "ConsoleApp"
	language "C++"

	-- Target and object directories with correct separators
	targetdir ("Binaries/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
	objdir ("Binaries/Intermediate/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")

	-- Add source files
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	-- Include necessary directories
	includedirs
	{
		"Graphite/vendor/spdlog/include",
		"Graphite/src",
	}

	-- Link Graphite to SandBox
	links
	{
		"Graphite"
	}

	-- Platform-specific settings
	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"GF_PLATFORM_WINDOWS",
		    "GLFW_STATIC"  -- Add this if using GLFW as a static library

		}

	-- Configuration-specific settings
	filter "configurations:Debug"
		defines "GF_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "GF_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "GF_DIST"
		optimize "On"
