cmake_minimum_required(VERSION 3.22)
find_program(DOTNET dotnet REQUIRED)

# --------------------------------------------------
# Dependency paths
# --------------------------------------------------

set(DEPS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../bin/Debug-linux-x86_64")

set(RAZOR_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Razor/src")
set(RAZOR_LIB_DIR "${DEPS_DIR}/Razor")

# Set this to the actual library filename for your platform.
if(WIN32)
    set(RAZOR_LIBRARY "${RAZOR_LIB_DIR}/Razor.lib")
elseif(APPLE)
    set(RAZOR_LIBRARY "${RAZOR_LIB_DIR}/libRazor.dylib")
else()
    set(RAZOR_LIBRARY "${RAZOR_LIB_DIR}/libRazor.so")
endif()

# --------------------------------------------------
# Precompiled Razor library
# --------------------------------------------------

if(NOT EXISTS "${RAZOR_LIBRARY}")
    message(FATAL_ERROR "Razor library not found: ${RAZOR_LIBRARY}")
endif()

if(NOT IS_DIRECTORY "${RAZOR_INCLUDE_DIR}")
    message(FATAL_ERROR "Razor headers not found: ${RAZOR_INCLUDE_DIR}")
endif()

add_library(Razor UNKNOWN IMPORTED GLOBAL)

set_target_properties(Razor PROPERTIES
    IMPORTED_LOCATION "${RAZOR_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${RAZOR_INCLUDE_DIR}"
)

# --------------------------------------------------
# C# scripts
# --------------------------------------------------

add_custom_target(SandboxScripts ALL
    COMMAND ${DOTNET} build
        "${CMAKE_CURRENT_SOURCE_DIR}/Scripts/Sandbox.csproj"
        -o "${CMAKE_CURRENT_SOURCE_DIR}/Scripts/Binaries/net9.0"
        -c $<CONFIG>
        --nologo
    COMMENT "Building Sandbox C# scripts"
    VERBATIM
)

file(GLOB_RECURSE SANDBOX_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

# Exclude Windows-only utilities on Linux
if(UNIX)
    list(FILTER SANDBOX_SOURCES EXCLUDE REGEX ".*/Utils/Windows/.*")
endif()

add_executable(Sandbox ${SANDBOX_SOURCES})

set_target_properties(Sandbox PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${OUTPUT_SUFFIX}/Sandbox"
)

target_compile_definitions(Sandbox PRIVATE
    $<$<PLATFORM_ID:Windows>:RZ_PLATFORM_WINDOWS>
    $<$<PLATFORM_ID:Linux>:RZ_PLATFORM_LINUX>
    $<$<CONFIG:Debug>:RZ_DEBUG>
    $<$<CONFIG:Release>:RZ_RELEASE>
    $<$<CONFIG:Dist>:RZ_DIST>
)

target_link_libraries(Sandbox PRIVATE
    Razor
)
