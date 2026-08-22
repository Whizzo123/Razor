# References for CMake Build System Migration

## Primary Reference: Premake Build Graph

### Root premake5.lua
- **Location**: `premake5.lua`
- **Relevance**: Authoritative source of workspace setup, configurations (Debug/Release/Dist), output dirs, and all three top-level projects
- **Key patterns to replicate**: Output dir format `bin/${cfg}-${system}-${arch}/`, pre-build commands for Jolt/Assimp/FreeType, post-build copies of Coral DLLs

### Razor project block (premake5.lua lines 40–305)
- **Relevance**: All include paths, link libraries, defines, and platform conditionals for the Razor shared library
- **Key patterns**: Linux `--whole-archive` for Jolt, per-config lib dirs, `RZ_BUILD_DLL` + platform defines

### Vendor premake5.lua files
- **Location**: `Razor/vendor/{GLFW,ImGui,yaml-cpp,entt,Coral/Coral.Native,Coral/Coral.Managed}/premake5.lua`
- **Relevance**: Source file lists and compiler settings for each vendored static lib
- **Note**: ImGui and Glad have no CMakeLists.txt — their premake5.lua is the reference for what sources and includes to include in the CMake wrapper

## Vendor CMake Integration

### JoltPhysics
- **Location**: `Razor/vendor/JoltPhysics/Build/CMakeLists.txt`
- **Relevance**: Existing CMake build; options: `JPH_DEBUG_RENDERER`, `CPP_RTTI_ENABLED`, `JPH_PROFILE_ENABLED`, `JPH_OBJECT_STREAM`

### Assimp
- **Location**: `Razor/vendor/assimp/CMakeLists.txt`
- **Relevance**: Standard CMake; disable tests and tools, static only

### GLFW
- **Location**: `Razor/vendor/GLFW/CMakeLists.txt`
- **Relevance**: `GLFW_BUILD_X11` / `GLFW_BUILD_WAYLAND` options map to Premake's `--display-backend` option

### Coral
- **Location**: `Razor/vendor/Coral/cmake/CMakeLists.txt`
- **Relevance**: Manages both Coral.Native (C++) and Coral.Managed (.NET) build
