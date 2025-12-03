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
	include "Razor/vendor/Coral/Coral.Native"
	include "Razor/vendor/Coral/Coral.Managed"

-----------------------------------------
-- External CMake library (managed by CMake, referenced by Premake)
-----------------------------------------
externalproject "Jolt"
    location "Razor/vendor/JoltPhysics/Build/VS2022_CL"   -- where the CMakeLists.txt lives
    kind "StaticLib"               -- or "SharedLib" if your CMake project outputs a DLL/SO
    language "C++"
	staticruntime "off"
	cppdialect "C++17"

    -- Optional: if you generate Visual Studio files, they appear here.
    -- Premake will NOT build this project. CMake will.

project "Razor"
	location "Razor"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	

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
		"%{prj.name}/vendor/entt/src",
		"%{prj.name}/vendor/Coral/Coral.Native/Include",
		"%{prj.name}/vendor/JoltPhysics/Include"
	}

	links
	{
		"GLFW",
		"opengl32.lib",
		"assimp",
		"ImGui",
		"yaml-cpp",
		"Coral.Native",
		"Jolt"
	}

	rtti("On")

	defines
	{
		"YAML_CPP_STATIC_DEFINE",
		"CORAL_WINDOWS"
	}

	 -- Automatically copy DLL from C# project after build
    postbuildcommands {
        '{COPY} "%{wks.location}Razor/vendor/Coral/Coral.Managed/bin/%{cfg.buildcfg}/Coral.Managed.dll" "%{wks.location}Edge/bin"',
		'{COPY} "%{wks.location}Razor/vendor/Coral/Coral.Managed/Coral.Managed.runtimeconfig.json" "%{wks.location}Edge/bin"',
		'{COPY} "%{cfg.buildtarget.relpath}" "%{wks.location}bin/' .. outputdir .. '/Edge"'
    }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"RZ_PLATFORM_WINDOWS",
			"CORAL_WINDOWS",
			"RZ_BUILD_DLL"
		}

	filter "configurations:Debug"
		prelinkcommands {
        	'if not exist "%{wks.location}\\Razor\\vendor\\JoltPhysics\\Build\\VS2022_CL\\Debug\\Jolt.lib" call "%{wks.location}\\Razor\\vendor\\JoltPhysics\\Build\\cmake_vs2022_cl.bat" -DUSE_STATIC_MSVC_RUNTIME_LIBRARY=OFF',
        	'if not exist "%{wks.location}\\Razor\\vendor\\JoltPhysics\\Build\\VS2022_CL\\Debug\\Jolt.lib" cmake --build "%{wks.location}\\Razor\\vendor\\JoltPhysics\\Build\\VS2022_CL" --config Debug'
    	}
		defines "RZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		prelinkcommands {
        	'if not exist "Razor\\vendor\\JoltPhysics\\Build\\VS2022_CL\\Release\\Jolt.lib" call "Razor\\vendor\\JoltPhysics\\Build\\cmake_vs2022_cl.bat"',
        	'if not exist "Razor\\vendor\\JoltPhysics\\Build\\VS2022_CL\\Release\\Jolt.lib" cmake --build "VS2022_CL" --config Release'
    	}
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
	cppdialect "C++20"
	staticruntime "off"

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
		"Razor/src",
		"Razor/vendor/entt/src"
	}
	
	links 
	{
		"Razor",
		"Shlwapi.lib",
		"Propsys.lib"
	}
	
	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

	filter "system:windows"
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
	cppdialect "C++20"
	staticruntime "off"

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
		"Razor/src",
		"Razor/vendor/entt/src"
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


	