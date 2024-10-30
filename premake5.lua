workspace "Graphite"
	architecture "x86_64"
	startproject "GraphEditor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Graphite/vendor/GLFW/include"
IncludeDir["Glad"] = "Graphite/vendor/Glad/include"
IncludeDir["ImGui"] = "Graphite/vendor/imgui"
IncludeDir["glm"] = "Graphite/vendor/glm"
IncludeDir["stb_image"] = "Graphite/vendor/stb_image"

group "Dependencies"
	include "Graphite/vendor/GLFW"
	include "Graphite/vendor/Glad"
	include "Graphite/vendor/imgui"
group ""

project "Graphite"
	location "Graphite"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "gfpch.h"
	pchsource "Graphite/src/gfpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

	filter "configurations:Debug"
		defines "GF_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GF_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "GF_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Graphite/vendor/spdlog/include",
		"Graphite/src",
		"Graphite/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Graphite"
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "GF_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GF_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "GF_DIST"
		runtime "Release"
		optimize "on"

project "GraphEditor"
	location "GraphEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Graphite/vendor/spdlog/include",
		"Graphite/src",
		"Graphite/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Graphite"
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "GF_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GF_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "GF_DIST"
		runtime "Release"
		optimize "on"