file(GLOB_RECURSE EDGE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

# Exclude Windows-only utilities on Linux
if(UNIX)
    list(FILTER EDGE_SOURCES EXCLUDE REGEX ".*/Utils/Windows/.*")
endif()

add_executable(Edge ${EDGE_SOURCES})

set_target_properties(Edge PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${OUTPUT_SUFFIX}/Edge"
)

target_compile_definitions(Edge PRIVATE
    YAML_CPP_STATIC_DEFINE
    $<$<PLATFORM_ID:Windows>:RZ_PLATFORM_WINDOWS>
    $<$<PLATFORM_ID:Linux>:RZ_PLATFORM_LINUX>
    $<$<CONFIG:Debug>:RZ_DEBUG>
    $<$<CONFIG:Release>:RZ_RELEASE>
    $<$<CONFIG:Dist>:RZ_DIST>
)

target_include_directories(Edge PRIVATE
    "${CMAKE_SOURCE_DIR}/Razor/vendor/spdlog/include"
    "${CMAKE_SOURCE_DIR}/Razor/vendor/GLFW/include"
    "${CMAKE_SOURCE_DIR}/Razor/vendor/Glad/include"
    "${CMAKE_SOURCE_DIR}/Razor/vendor/stb_image/include"
    "${CMAKE_SOURCE_DIR}/Razor/vendor/glm"
    "${CMAKE_SOURCE_DIR}/Razor/vendor/ImGui"
    "${CMAKE_SOURCE_DIR}/Razor/vendor/yaml-cpp/include"
    "${CMAKE_SOURCE_DIR}/Razor/vendor/entt/src"
)

target_link_libraries(Edge PRIVATE
    Razor
    $<$<PLATFORM_ID:Windows>:Shlwapi;Propsys>
)
