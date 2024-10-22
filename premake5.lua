workspace "Graphite"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

-- Define the output directory structure using tokens
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Graphite"
	location "Graphite"
	kind "SharedLib"
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

	-- Correct includedirs syntax, removing potential curly brace errors
	includedirs
	{
		"%{prj.name}/src"
		"%{prj.name}/vendor/spdlog/include"
	}

	-- Platform-specific settings
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"GF_PLATFORM_WINDOWS",
			"GF_BUILD_DLL"
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
		"Graphite/src"
	}

	-- Link Graphite to SandBox
	links
	{
		"Graphite"
	}

	-- Platform-specific settings
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"GF_PLATFORM_WINDOWS"
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
