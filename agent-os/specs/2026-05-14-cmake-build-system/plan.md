# CMake Build System Migration — Full Plan

See `/home/joseph.scobbie/.claude/plans/precious-jumping-unicorn.md` for the complete implementation plan.

## Summary

Add CMake alongside Premake5 for Razor. Targets: Linux (Ninja), Windows (Ninja/MSVC). Modern target-based CMake throughout.

## New Files

| File | Purpose |
|---|---|
| `CMakeLists.txt` | Root workspace |
| `Razor/CMakeLists.txt` | Razor shared library |
| `Edge/CMakeLists.txt` | Edge editor executable |
| `Razor-ScriptBridge/CMakeLists.txt` | dotnet build wrapper |
| `Sandbox/CMakeLists.txt` | Sandbox C# scripts dotnet build |
| `Razor/vendor/ImGui/CMakeLists.txt` | ImGui static lib wrapper |
| `Razor/vendor/Glad/CMakeLists.txt` | Glad static lib wrapper |
| `GenerateProjects-cmake.sh` | Linux generator script |
| `GenerateProjects-cmake.bat` | Windows generator script |

## Unchanged

All existing Premake files (`premake5.lua`, `GenerateProjects.sh`, `GenerateProjects.bat`) remain intact.
