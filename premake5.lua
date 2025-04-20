workspace "Razor"
	architecture "x64"
	startproject "Sandbox"

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
	include "Razor/vendor/yaml-cpp"
	include "Razor/vendor/entt"

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
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/entt/src**.hpp",
		"%{prj.name}/vendor/entt/src**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs 
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/assimp/include",
		"%{prj.name}/vendor/Glad/include",
		"%{prj.name}/vendor/stb_image/include",
		"%{prj.name}/vendor/glm",
		"%{prj.name}/vendor/ImGui",
		"%{prj.name}/vendor/yaml-cpp/include",
		"%{prj.name}/vendor/entt/src"
	}

	links
	{
		"GLFW",
		"opengl32.lib",
		"assimp",
		"ImGui",
		"yaml-cpp"
	}

	rtti("On")

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

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

project "Edge"
	location "Edge"
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
		"Razor/vendor/ImGui",
		"Razor/vendor/yaml-cpp/include",
		"Razor/src"
	}
	
	links 
	{
		"Razor"
	}
	
	defines
	{
		"YAML_CPP_STATIC_DEFINE"
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
		"Razor/vendor/ImGui",
		"Razor/vendor/yaml-cpp/include",
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
			"RZ_PLATFORM_WINDOWS",
			"YAML_CPP_STATIC_DEFINE"
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


	