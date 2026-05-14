file(GLOB_RECURSE RAZOR_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

add_library(Razor SHARED ${RAZOR_SOURCES})

set_target_properties(Razor PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${OUTPUT_SUFFIX}/Razor"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${OUTPUT_SUFFIX}/Razor"
)

# RTTI required by JoltPhysics
target_compile_options(Razor PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/GR>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-frtti>
)

# Per-config optimization
target_compile_options(Razor PRIVATE
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Release>>:-O2>
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Dist>>:-O3>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<OR:$<CONFIG:Release>,$<CONFIG:Dist>>>:/O2>
)

# Global defines
target_compile_definitions(Razor PRIVATE
    _CRT_SECURE_NO_WARNINGS
    JPH_PROFILE_ENABLED
    JPH_DEBUG_RENDERER
    JPH_OBJECT_STREAM
    YAML_CPP_STATIC_DEFINE
    RZ_BUILD_DLL
)

# Platform defines
target_compile_definitions(Razor PRIVATE
    $<$<PLATFORM_ID:Windows>:RZ_PLATFORM_WINDOWS;CORAL_WINDOWS;JPH_FLOATING_POINT_EXCEPTIONS_ENABLED>
    $<$<PLATFORM_ID:Linux>:RZ_PLATFORM_LINUX;CORAL_LINUX>
)

# GLFW backend define
if(RAZOR_GLFW_BACKEND STREQUAL "wayland")
    target_compile_definitions(Razor PRIVATE RZ_GLFW_WAYLAND)
else()
    target_compile_definitions(Razor PRIVATE RZ_GLFW_X11)
endif()

# Include directories for header-only and non-target deps
target_include_directories(Razor PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_include_directories(Razor PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/entt/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/JoltPhysics"
)

# Vendor targets (propagate includes via PUBLIC interface of each target)
target_link_libraries(Razor PUBLIC
    glfw
    ImGui
    Glad
    yaml-cpp
    Coral.Native
    assimp
    ${FREETYPE_TARGET}
)

# Jolt: wrap in --whole-archive on Linux to pull all symbols
if(UNIX AND NOT APPLE)
    target_link_options(Razor PRIVATE
        -Wl,--whole-archive $<TARGET_FILE:Jolt> -Wl,--no-whole-archive
    )
    add_dependencies(Razor Jolt)
else()
    target_link_libraries(Razor PRIVATE Jolt)
endif()

# Platform link libraries
target_link_libraries(Razor PRIVATE
    $<$<PLATFORM_ID:Windows>:opengl32>
    $<$<PLATFORM_ID:Linux>:GL;dl;pthread;z>
)

if(RAZOR_GLFW_BACKEND STREQUAL "wayland")
    target_link_libraries(Razor PRIVATE EGL wayland-client wayland-egl wayland-cursor)
else()
    target_link_libraries(Razor PRIVATE
        $<$<PLATFORM_ID:Linux>:X11;Xrandr;Xi;Xcursor>
    )
endif()

# Post-build: copy Razor .so + Coral.Managed DLL to Edge output dir
set(EDGE_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/${OUTPUT_SUFFIX}/Edge")
set(CORAL_BUILD_DIR "${CMAKE_BINARY_DIR}/Razor/vendor/Coral/cmake")

add_custom_command(TARGET Razor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${EDGE_OUTPUT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:Razor>"
        "${EDGE_OUTPUT_DIR}/$<TARGET_FILE_NAME:Razor>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CORAL_BUILD_DIR}/Coral.Managed.dll"
        "${EDGE_OUTPUT_DIR}/Coral.Managed.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CORAL_BUILD_DIR}/Coral.Managed.runtimeconfig.json"
        "${EDGE_OUTPUT_DIR}/Coral.Managed.runtimeconfig.json"
    COMMENT "Copying Razor and Coral managed DLL to Edge output"
)
