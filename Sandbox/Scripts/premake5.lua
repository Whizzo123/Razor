local RazorRootDir = '../../'
include (RazorRootDir .. "/vendor/premake/premake_customization/solution_items.lua")

workspace "Sandbox"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Sandbox"

    filter { "not action:vs*", "not system:windows" }
        kind "StaticLib"
        language "C++"
        files { "Source/Dummy.cpp" }
        prebuildcommands {
            'dotnet build "%{prj.location}/Sandbox.csproj"' ..
            ' -c %{cfg.buildcfg}' ..
            ' -o "%{prj.location}/Binaries/net9.0"' ..
            ' --nologo'
        }

    filter { "action:vs* or system:windows" }
        kind "SharedLib"
        language "C#"
        dotnetframework "net9.0"
        targetdir ("Binaries")
        objdir ("Intermediates")
        architecture "x86_64"
        files { "Source/**.cs", "Properties/**.cs" }
        links { "Razor-ScriptBridge" }

    filter "configurations:Debug"
        optimize "Off"
        symbols "Default"

    filter "configurations:Release"
        optimize "On"
        symbols "Default"

    filter "configurations:Dist"
        optimize "Full"
        symbols "Off"

group "Razor"
	include (RazorRootDir .. "/Razor-ScriptBridge")
group ""
