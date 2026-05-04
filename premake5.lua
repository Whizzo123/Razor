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
	include "Razor/vendor/ImGui"
	include "Razor/vendor/yaml-cpp"
	include "Razor/vendor/entt"
	include "Razor/vendor/Coral/Coral.Native"
	include "Razor/vendor/Coral/Coral.Managed"

-----------------------------------------
-- External CMake library (managed by CMake, referenced by Premake)
-----------------------------------------

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
		"JPH_PROFILE_ENABLED",
		"JPH_DEBUG_RENDERER",
		"JPH_OBJECT_STREAM"
	}

	includedirs 
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/assimp/include",
		"%{prj.name}/vendor/assimp/build/include",
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
			"%{prj.name}/vendor/JoltPhysics/Build/Linux_Debug",
			"%{prj.name}/vendor/assimp/bin/" .. outputdir .. "/assimp"
		}
		

	filter "system:windows"

	libdirs
	{	
		"%{prj.name}/vendor/JoltPhysics/Build/VS2022_CL/Debug",
		"%{prj.name}/vendor/assimp/bin/" .. outputdir .. "/assimp"
	}

	defines
	{
		"JPH_FLOATING_POINT_EXCEPTIONS_ENABLED"
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

	filter "system:linux"
	    links
	    {
			"GL",
	        "dl",
	        "pthread",
			"assimp",
	        "GLFW",
			"EGL",
			"wayland-client",
			"wayland-egl",
        	"wayland-cursor",
	        "ImGui",
	        "yaml-cpp",
	        "Coral.Native",
	        "Jolt",
			"z"
	    }
	 	linkoptions
    	{
        	"-Wl,--whole-archive",
        	"-lJolt",
        	"-Wl,--no-whole-archive"
    	}
	filter {}
	rtti("On")

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

	 -- Automatically copy DLL from C# project after build
    filter "system:windows"
    	postbuildcommands {
    	    '{COPY} "%{cfg.buildtarget.relpath}" "%{wks.location}bin/' .. outputdir .. '/Edge"',
    	    '{COPY} "%{wks.location}Razor/vendor/Coral/Build/%{cfg.buildcfg}/Coral.Managed.dll" "%{wks.location}bin/' .. outputdir .. '/Edge"',
    	    '{COPY} "%{wks.location}Razor/vendor/Coral/Build/%{cfg.buildcfg}/Coral.Managed.runtimeconfig.json" "%{wks.location}bin/' .. outputdir .. '/Edge"'
    	}

	filter "system:linux"
    	postbuildcommands {
    	    '{COPY} "Razor/vendor/Coral/Coral.Managed/bin/%{cfg.buildcfg}/libCoral.Managed.a" "%{wks.location}Edge/bin"',
    	    '{COPY} "%{cfg.buildtarget.relpath}" "%{wks.location}bin/' .. outputdir .. '/Edge"',
    	    '{COPY} "%{wks.location}/Razor/vendor/Coral/Coral.Managed/Coral.Managed.runtimeconfig.json" "%{wks.location}bin/' .. outputdir .. '/Edge"'
    	}

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
		pic "On"
		defines
		{
			"RZ_BUILD_DLL",
			"RZ_PLATFORM_LINUX",
			"CORAL_LINUX"
		}
		

	filter {"configurations:Debug", "system:windows"}
		prebuildcommands {
	    	'if not exist "%{wks.location}\\Razor\\vendor\\JoltPhysics\\Build\\VS2022_CL\\Debug\\Jolt.lib" call "%{wks.location}\\Razor\\vendor\\JoltPhysics\\Build\\cmake_vs2022_cl.bat" -DUSE_STATIC_MSVC_RUNTIME_LIBRARY=OFF',
    		'if not exist "%{wks.location}\\Razor\\vendor\\JoltPhysics\\Build\\VS2022_CL\\Debug\\Jolt.lib" cmake --build "%{wks.location}\\Razor\\vendor\\JoltPhysics\\Build\\VS2022_CL" --config Debug'
    	}
	filter {"configurations:Debug", "system:linux"}
		prebuildcommands {
	    	 -- Configure step (only if build dir does not exist)
        	'if [ ! -f "%{wks.location}/Razor/vendor/JoltPhysics/Build/Linux_Debug" ]; then ' ..
			'(cd "%{wks.location}/Razor/vendor/JoltPhysics/Build" && ' ..
        	'sh ./cmake_linux_clang_gcc.sh Debug g++ -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCPP_RTTI_ENABLED=ON -DJPH_DEBUG_RENDERER=ON); fi',
        	-- Build step
			'if [ ! -f "%{wks.location}/Razor/vendor/JoltPhysics/Build/Linux_Debug/libjolt.a" ]; then ' ..
        	'cmake --build "%{wks.location}/Razor/vendor/JoltPhysics/Build/Linux_Debug"; fi',
			-- Assimp configure step (only if build dir does not exist)
			'if [ ! -d "%{wks.location}/Razor/vendor/assimp/build" ]; then ' ..
			'cmake -S "%{wks.location}/Razor/vendor/assimp" -B "%{wks.location}/Razor/vendor/assimp/build" ' ..
			'-DCMAKE_POSITION_INDEPENDENT_CODE=ON -DBUILD_SHARED_LIBS=OFF -DASSIMP_BUILD_TESTS=OFF ' ..
			'-DASSIMP_BUILD_ASSIMP_TOOLS=OFF; fi',
			-- Assimp build step
			'if [ ! -f "%{wks.location}/Razor/vendor/assimp/bin/' .. outputdir .. '/assimp/libassimp.a" ]; then ' ..
        	'cmake --build "%{wks.location}/Razor/vendor/assimp/build" && ' ..
			'mkdir -p "%{wks.location}/Razor/vendor/assimp/bin/' .. outputdir .. '/assimp" && ' ..
			'cp "%{wks.location}/Razor/vendor/assimp/build/lib/libassimp.a" "%{wks.location}/Razor/vendor/assimp/bin/' .. outputdir .. '/assimp/libassimp.a"; fi'
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
		"Razor/vendor/assimp/build/include",
		"Razor/vendor/Glad/include",
		"Razor/vendor/stb_image/include",
		"Razor/vendor/glm",
		"Razor/vendor/ImGui",
		"Razor/vendor/yaml-cpp/include",
		"Razor/src",
		"Razor/vendor/entt/src"
	}
	
	filter "system:windows"
		links 
		{
			"Razor",
			"Shlwapi.lib",
			"Propsys.lib"
		}

	filter "system:linux"
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

		defines
		{
			"RZ_PLATFORM_WINDOWS"
		}
	
	filter "system:linux"
		removefiles {
    		"%{prj.name}/src/Utils/Windows**.h",
			"%{prj.name}/src/Utils/Windows**.cpp"
		}
		defines
		{
			"RZ_PLATFORM_LINUX"
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


	