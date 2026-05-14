find_program(DOTNET dotnet REQUIRED)

add_custom_target(Razor-ScriptBridge ALL
    COMMAND ${DOTNET} build
        "${CMAKE_CURRENT_SOURCE_DIR}/Razor-ScriptBridge.csproj"
        -o "${CMAKE_SOURCE_DIR}/Edge/Resources/Scripts"
        -c $<CONFIG>
        --nologo
    COMMENT "Building Razor-ScriptBridge C# project"
    VERBATIM
)

add_dependencies(Razor Razor-ScriptBridge)
