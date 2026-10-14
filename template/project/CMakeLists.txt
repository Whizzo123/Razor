find_program(DOTNET dotnet REQUIRED)

add_custom_target(@ProjectName ALL
    COMMAND ${DOTNET} build
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/@ProjectName.csproj"
        -o "${CMAKE_CURRENT_SOURCE_DIR}/scripts/Binaries/net9.0"
        -c $<CONFIG>
        --nologo
    COMMENT "Building @ProjectName C# scripts"
    VERBATIM
)

add_dependencies(@ProjectName Razor-ScriptBridge)
