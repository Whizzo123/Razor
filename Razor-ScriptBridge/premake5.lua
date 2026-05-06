
project "Razor-ScriptBridge"
        kind "SharedLib"
        language "C#"
        dotnetframework "net9.0"
        clr "UnSafe"
        
        targetdir("../Edge/Resources/Scripts")
        objdir("../Edge/Resources/Scripts/Intermediates")

        architecture "x86_64"

        files 
        {
            "Source/**.cs",
            "Properties/**.cs"
        }

        filter "configurations:Debug"
            optimize "Off"
            symbols "Default"

        filter "configurations:Release"
            optimize "On"
            symbols "Default"

        filter "configurations:Dist"
            optimize "Full"
            symbols "Off"