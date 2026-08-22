# CMake Build System Migration — Shaping Notes

## Scope

Add CMake as a first-class build system for Razor, targeting Linux (Makefile/Ninja) and Windows (Ninja/MSVC). Premake5 is deprecated but not deleted — both systems must coexist. The CMake build replicates the Premake build graph exactly: Razor (SharedLib), Edge (ConsoleApp), Razor-ScriptBridge (dotnet), Sandbox C# scripts.

## Decisions

- **New scripts, not replacements**: Add `GenerateProjects-cmake.sh` / `.bat` alongside existing Premake scripts. Premake files remain untouched.
- **Modern, target-based CMake**: Use `target_link_libraries`, `target_include_directories`, generator expressions — avoid global include_directories and add_definitions.
- **`add_subdirectory` for vendored deps that have CMakeLists.txt**: GLFW, yaml-cpp, JoltPhysics, Assimp, Coral all have their own CMakeLists.txt and will be integrated this way.
- **Wrapper CMakeLists.txt for ImGui and Glad**: Neither has a CMakeLists.txt. Thin wrappers created in their vendor directories.
- **INTERFACE targets for header-only deps**: spdlog, glm, entt — no compilation needed, just include paths.
- **Custom `Dist` config**: CMake supports custom build types. Dist = full optimization + RZ_DIST define, mirrors the Premake 3-config model.
- **Output paths mirror Premake**: `bin/Debug-linux-x86_64/` etc. keeps existing asset path references working.
- **Jolt `--whole-archive` via `target_link_options`**: Required on Linux so all Jolt symbols are included.
- **FreeType**: Submodule present but may need `git submodule update --init --recursive` — same requirement as Premake.

## Context

- **Visuals**: None — build system change
- **References**: Existing `premake5.lua` (root), vendor `premake5.lua` files, `GenerateProjects.sh`
- **Product alignment**: tech-stack.md lists Premake5 as build system — will need updating to reflect CMake addition
