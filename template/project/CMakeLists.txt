find_program(DOTNET dotnet REQUIRED)

add_custom_target(Sandbox ALL
    COMMAND ${DOTNET} build
        "${CMAKE_CURRENT_SOURCE_DIR}/Scripts/Sandbox.csproj"
        -o "${CMAKE_CURRENT_SOURCE_DIR}/Scripts/Binaries/net9.0"
        -c $<CONFIG>
        --nologo
    COMMENT "Building Sandbox C# scripts"
    VERBATIM
)

add_dependencies(Sandbox Razor-ScriptBridge)
