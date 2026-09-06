cmake_minimum_required(VERSION 3.22)
project(Razor VERSION 1.0 LANGUAGES C CXX)

# Allow vendored CMakeLists.txt files with cmake_minimum_required < 3.5
# (yaml-cpp uses 3.4, removed from CMake 4.x without this override)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Output dirs mirror Premake: bin/${cfg}-${system}-${arch}/
string(TOLOWER "${CMAKE_SYSTEM_NAME}" SYSTEM_LOWER)
set(OUTPUT_SUFFIX "${CMAKE_BUILD_TYPE}-${SYSTEM_LOWER}-x86_64")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${OUTPUT_SUFFIX}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${OUTPUT_SUFFIX}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${OUTPUT_SUFFIX}")

# GLFW display backend (Linux)
set(RAZOR_GLFW_BACKEND "x11" CACHE STRING "GLFW display backend: x11 or wayland")
set_property(CACHE RAZOR_GLFW_BACKEND PROPERTY STRINGS "x11" "wayland")

# Build-type defines applied globally
add_compile_definitions(
    $<$<CONFIG:Debug>:RZ_DEBUG>
    $<$<CONFIG:Release>:RZ_RELEASE>
    $<$<CONFIG:Dist>:RZ_DIST>
)

# ---- Vendor options -------------------------------------------------------

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

if(UNIX AND NOT APPLE)
    if(RAZOR_GLFW_BACKEND STREQUAL "wayland")
        set(GLFW_BUILD_WAYLAND ON  CACHE BOOL "" FORCE)
        set(GLFW_BUILD_X11     OFF CACHE BOOL "" FORCE)
    else()
        set(GLFW_BUILD_X11     ON  CACHE BOOL "" FORCE)
        set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
    endif()
endif()

# JoltPhysics
set(JPH_DEBUG_RENDERER        ON  CACHE BOOL "" FORCE)
set(JPH_PROFILE_ENABLED       ON  CACHE BOOL "" FORCE)
set(JPH_OBJECT_STREAM         ON  CACHE BOOL "" FORCE)
set(CPP_RTTI_ENABLED          ON  CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS         OFF CACHE BOOL "" FORCE)
set(TARGET_UNIT_TESTS         OFF CACHE BOOL "" FORCE)
set(TARGET_PERFORMANCE_TEST   OFF CACHE BOOL "" FORCE)
set(INTERPROCEDURAL_OPTIMIZATION OFF CACHE BOOL "" FORCE)
set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF CACHE BOOL "" FORCE)

# Assimp
set(ASSIMP_BUILD_TESTS          OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS   OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL              OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT            ON  CACHE BOOL "" FORCE)
# Prevent assimp from setting CMAKE_DEBUG_POSTFIX="d" globally
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS   OFF CACHE BOOL "" FORCE)

# yaml-cpp
set(YAML_CPP_BUILD_TOOLS      OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TESTS      OFF CACHE BOOL "" FORCE)
set(YAML_BUILD_SHARED_LIBS    OFF CACHE BOOL "" FORCE)

# Coral
set(CORAL_EXAMPLE  OFF CACHE BOOL "" FORCE)
set(CORAL_TESTING  OFF CACHE BOOL "" FORCE)

# ---- MSVC -----------------------------------------------------------------

if(MSVC)
    add_compile_options(/EHsc)
    # Use the DLL MSVC runtime:
    # Debug   -> /MDd
    # Release -> /MD
    # This must be consistent across Razor and all static dependencies.
    set(CMAKE_MSVC_RUNTIME_LIBRARY
        "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
        CACHE STRING "MSVC runtime library" FORCE
    )
endif()

# ---- Vendor subdirectories ------------------------------------------------

add_subdirectory(Razor/vendor/GLFW)
add_subdirectory(Razor/vendor/ImGui)
target_sources(ImGui PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Razor/src/Platform/OpenGL/imgui_impl_glfw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Razor/src/Platform/OpenGL/imgui_impl_opengl3.cpp"
)
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        target_compile_options(ImGui PRIVATE
            -Wno-nontrivial-memcall 
        )
    endif()
    if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
        target_compile_options(ImGui PRIVATE
            -Wno-nontrivial-memaccess
        )
    endif()
endif()

add_subdirectory(Razor/vendor/Glad)
add_subdirectory(Razor/vendor/yaml-cpp)
add_subdirectory(Razor/vendor/JoltPhysics/Build)
add_subdirectory(Razor/vendor/assimp)
add_subdirectory(Razor/vendor/Coral)

# FreeType: use submodule if initialized, otherwise fall back to system package
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Razor/vendor/freetype/CMakeLists.txt")
    add_subdirectory(Razor/vendor/freetype)
    set(FREETYPE_TARGET freetype)
else()
    find_package(Freetype REQUIRED)
    set(FREETYPE_TARGET Freetype::Freetype)
endif()

# ---- Project warning policy ----------------------------------------------

add_library(RazorWarnings INTERFACE)

if(MSVC)
    target_compile_options(RazorWarnings INTERFACE
        /W4
        /WX
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(RazorWarnings INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(RazorWarnings INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )
endif()

# ---- Projects -------------------------------------------------------------

add_subdirectory(Razor)
add_subdirectory(Edge)
add_subdirectory(Razor-ScriptBridge)
add_subdirectory(Sandbox)

# ---- Clean targets --------------------------------------------------------

add_custom_target(clean-vendor
    COMMAND ${CMAKE_COMMAND} -E rm -rf
        "${CMAKE_CURRENT_SOURCE_DIR}/Razor/vendor/JoltPhysics/Build/VS2022_CL"
        "${CMAKE_CURRENT_SOURCE_DIR}/Razor/vendor/assimp/bin"
        "${CMAKE_CURRENT_SOURCE_DIR}/Razor/vendor/freetype/build"
    COMMENT "Removing vendor build artifacts..."
)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E rm -rf
        "${CMAKE_CURRENT_SOURCE_DIR}/bin"
        "${CMAKE_CURRENT_SOURCE_DIR}/Razor/vendor/JoltPhysics/Build/VS2022_CL"
        "${CMAKE_CURRENT_SOURCE_DIR}/Razor/vendor/assimp/bin"
        "${CMAKE_CURRENT_SOURCE_DIR}/Razor/vendor/freetype/build"
    COMMENT "Removing Razor and vendor build artifacts..."
)