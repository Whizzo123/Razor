workspace "Razor"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Razor/vendor/GLFW"
	include "Razor/vendor/assimp"
	include "Razor/vendor/ImGui"

project "Razor"
	location "Razor"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/Glad/src/glad.c",
		"%{prj.name}/vendor/stb_image/src/stb_image.h",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs 
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/assimp/include",
		"%{prj.name}/vendor/Glad/include",
		"%{prj.name}/vendor/stb_image/include",
		"%{prj.name}/vendor/glm",
		"%{prj.name}/vendor/ImGui"
	}

	links
	{
		"GLFW",
		"opengl32.lib",
		"assimp",
		"ImGui"
	}

	rtti("On")

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"RZ_BUILD_DLL",
			"RZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "RZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "RZ_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"Razor/vendor/spdlog/include",
		"Razor/vendor/GLFW/include",
		"Razor/vendor/assimp/include",
		"Razor/vendor/Glad/include",
		"Razor/vendor/stb_image/include",
		"Razor/vendor/glm",
		"Razor/src"
	}

	links 
	{
		"Razor"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "10.0.22621.0"

		defines
		{
			"RZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "RZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "RZ_DIST"
		runtime "Release"
		optimize "on"


	