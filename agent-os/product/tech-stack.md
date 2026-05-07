# Tech Stack

## Language & Standard

- **C++20** — engine and editor
- **C#/.NET 9** — game scripting layer (Sandbox scripts, script bridge)

## Rendering

- **OpenGL** — rendering backend
- **GLFW** — window and input management
- **Dear ImGui** — editor UI

## Entity Component System

- **EnTT** — ECS registry, component storage, and entity management

## Physics

- **JoltPhysics** — rigid body simulation, built from source with RTTI enabled

## Scripting Bridge

- **Coral** — .NET assembly hosting; allows C# scripts to attach to entities at runtime

## Serialization

- **yaml-cpp** — scene and project file serialization

## Build System

- **Premake5** — generates Visual Studio (Windows) and Makefile (Linux) project files
- **MSBuild / make** — build execution
- JoltPhysics and Assimp are built from source via CMake pre-build steps

## Asset Pipeline

- **Assimp** — mesh and scene asset importing (built from source)

## Platform Support

- **Windows** — primary platform (Visual Studio 2022)
- **Linux** — supported (GLFW Wayland, gmake build)

## Output Structure

- Engine compiled as a shared library (DLL on Windows, .so on Linux)
- Editor: `Edge` application
- Test/demo: `Sandbox` application
- Build output: `bin/[Config]-[system]-x86_64/[Project]/`
