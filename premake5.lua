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

-- Coral.Managed uses .NET - include premake only on Windows, use dotnet on Linux
if os.target() == "windows" then
	include "Razor/vendor/Coral/Coral.Managed"
end

-----------------------------------------
-- External CMake library (managed by CMake, referenced by Premake)
-----------------------------------------
project "Jolt"
	kind "None"

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
		"%{prj.name}/vendor/entt/src**.cpp",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"JPH_FLOATING_POINT_EXCEPTIONS_ENABLED",
		"JPH_PROFILE_ENABLED",
		"JPH_DEBUG_RENDERER",
		"JPH_OBJECT_STREAM"
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
		"%{prj.name}/vendor/JoltPhysics"
	}

	filter "system:linux"
		libdirs
		{	
			"%{prj.name}/vendor/JoltPhysics/Build/Linux_Debug"
		}
	filter "system:windows"
		libdirs
		{	
			"%{prj.name}/vendor/JoltPhysics/Build/VS_2022_CL"
		}
	filter {}

	filter "system:windows"
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

	filter "system:linux"
		links
		{
			"GLFW",
			"GL",
			"assimp",
			"ImGui",
			"yaml-cpp",
			"Coral.Native",
			"Jolt",
			"dl",
			"pthread",
			"X11"
		}

	filter {}

	rtti("On")

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

	filter "system:windows"
		defines { "CORAL_WINDOWS" }

		 -- Automatically copy DLL from C# project after build
		postbuildcommands {
			'{COPY} "%{wks.location}Razor/vendor/Coral/Coral.Managed/bin/%{cfg.buildcfg}/Coral.Managed.dll" "%{wks.location}Edge/bin"',
			'{COPY} "%{wks.location}Razor/vendor/Coral/Coral.Managed/Coral.Managed.runtimeconfig.json" "%{wks.location}Edge/bin"',
			'{COPY} "%{cfg.buildtarget.relpath}" "%{wks.location}bin/' .. outputdir .. '/Edge"'
		}

	filter "system:linux"
		defines { "CORAL_LINUX" }

		-- Copy Coral.Managed DLL after build
		postbuildcommands {
			'mkdir -p "%{wks.location}/bin/' .. outputdir .. '/Edge" && ' ..
			'{COPY} "%{wks.location}/Razor/vendor/Coral/Coral.Managed/bin/%{cfg.buildcfg}/net9.0/Coral.Managed.dll" "%{wks.location}/bin/' .. outputdir .. '/Edge/"'
		}

	filter {}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"RZ_PLATFORM_WINDOWS",
			"CORAL_WINDOWS",
			"RZ_BUILD_DLL"
		}
	
	filter "system:linux"
		systemversion "latest"

		defines
		{
			"RZ_BUILD_DLL",
			"RZ_PLATFORM_LINUX"
		}

	filter {"configurations:Debug", "system:windows"}
		prebuildcommands {
	    	'if not exist "%{wks.location}\\Razor\\vendor\\JoltPhysics\\Build\\VS2022_CL\\Debug\\Jolt.lib" call "%{wks.location}\\Razor\\vendor\\JoltPhysics\\Build\\cmake_vs2022_cl.bat" -DUSE_STATIC_MSVC_RUNTIME_LIBRARY=OFF',
    		'if not exist "%{wks.location}\\Razor\\vendor\\JoltPhysics\\Build\\VS2022_CL\\Debug\\Jolt.lib" cmake --build "%{wks.location}\\Razor\\vendor\\JoltPhysics\\Build\\VS2022_CL" --config Debug'
    	}
	filter {"configurations:Debug", "system:linux"}
		prebuildcommands {
	    	 -- Configure step for JoltPhysics (only if build dir does not exist)
        	'if [ ! -f "%{wks.location}/Razor/vendor/JoltPhysics/Build/Linux_Debug/Makefile" ]; then ' ..
			'(cd "%{wks.location}/Razor/vendor/JoltPhysics/Build" && ' ..
        	'sh ./cmake_linux_clang_gcc.sh Debug g++ -DUSE_STATIC_MSVC_RUNTIME_LIBRARY=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=ON); fi',
        	-- Build step for JoltPhysics
			'if [ ! -f "%{wks.location}/Razor/vendor/JoltPhysics/Build/Linux_Debug/libjolt.a" ]; then ' ..
        	'cmake --build "%{wks.location}/Razor/vendor/JoltPhysics/Build/Linux_Debug"; fi',
			-- Build Coral.Managed using dotnet
			'dotnet build "%{wks.location}/Razor/vendor/Coral/Coral.Managed/Coral.Managed-Static.csproj" -c Debug -o "%{wks.location}/Razor/vendor/Coral/Coral.Managed/bin/Debug/net9.0"'
    	}
	filter "configurations:Debug"
		defines "RZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		prebuildcommands {
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
		"Razor"
	}
	
	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

	filter "system:windows"
		systemversion "10.0.22621.0"

		links
		{
			"Shlwapi.lib",
			"Propsys.lib"
		}

		defines
		{
			"RZ_PLATFORM_WINDOWS"
		}

	filter "system:linux"
		systemversion "latest"

		links
		{
			"GLFW",
			"assimp",
			"ImGui",
			"yaml-cpp",
			"GL",
			"dl",
			"pthread",
			"X11"
		}

		defines
		{
			"RZ_PLATFORM_LINUX"
		}

	filter {}
	
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

	filter "system:linux"
		systemversion "latest"

		links
		{
			"GLFW",
			"assimp",
			"ImGui",
			"yaml-cpp",
			"GL",
			"dl",
			"pthread",
			"X11"
		}

		defines
		{
			"RZ_PLATFORM_LINUX"
		}

	filter {}

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


	