# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What This Is

Razor is a 3D game engine (C++20, OpenGL) with two applications built on top of it:
- **Edge** — the visual scene editor
- **Sandbox** — a test/demo application

The engine is compiled as a shared library (DLL on Windows, .so on Linux).

## Build System

Razor uses **Premake5** to generate IDE project files.

**Prerequisites:**
- Premake5 binary at `vendor/bin/premake/premake5.exe` (Windows) or `vendor/bin/premake/premake5` (Linux)
- All git submodules initialized: `git submodule update --init --recursive`
- JoltPhysics and Assimp are built automatically as pre-build steps on first compile

**Windows (Visual Studio 2022):**
```bat
GenerateProjects.bat          # generates Razor.sln
GenerateProjects.bat vs2019   # alternate generator
```
Then open `Razor.sln` in Visual Studio and build.

**Linux (Makefiles):**
```sh
./GenerateProjects.sh         # generates Makefiles via gmake
make config=debug
```

**Build configurations:** `Debug`, `Release`, `Dist`

**Output paths:** `bin/[Config]-[system]-x64/[Project]/`

There is no standalone test suite — testing is done by running Edge.

## Testing Workflow (Windows)

After writing any feature or fix, always build and run Edge to verify:

**1. Build** (run from workspace root via PowerShell):
```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" "C:\Sandbox\Razor\Razor.sln" /p:Configuration=Debug /m /nologo
```

**2. Run** (from workspace root so asset paths resolve):
```powershell
Start-Process "C:\Sandbox\Razor\bin\Debug-windows-x86_64\Edge\Edge.exe" -WorkingDirectory "C:\Sandbox\Razor"
```

**3. Verify:**
- A non-zero MSBuild exit code means a build regression — fix before reporting done.
- Edge opens a window on the desktop. Confirm it reaches the editor UI without crashing.
- Console/spdlog output streams to stdout and can be captured if needed.

**Notes:**
- If new files were added to the project, run `GenerateProjects.bat` first to regenerate `Razor.sln` via Premake.
- Build output is at `bin/Debug-windows-x86_64/` (Premake uses `x86_64`, not `x64`).
- Coral.Managed builds to `Razor/vendor/Coral/Build/Debug/` — this is already referenced correctly in `premake5.lua` after the fix on 2026-05-04.

## Testing Workflow (Linux)

After writing any feature or fix, always build and run Edge to verify:

**1. Build** (run from workspace root):
```sh
make -j$(nproc) config=debug
```

**2. Run** (from workspace root so asset paths resolve):
```sh
./bin/Debug-linux-x86_64/Edge/Edge
```

**3. Verify:**
- A non-zero make exit code means a build regression — fix before reporting done.
- Edge opens a window on the desktop. Confirm it reaches the editor UI without crashing.
- Console/spdlog output streams to stdout and can be captured if needed.

**Notes:**
- If new files were added to the project, run `./GenerateProjects.sh` first to regenerate Makefiles via Premake.
- Build output is at `bin/Debug-linux-x86_64/`.
- GLFW is configured for Wayland by default; ensure a Wayland compositor is running or set `WAYLAND_DISPLAY` appropriately.

## Architecture

### Engine Layer (`Razor/src/`)

The engine is built around a few core systems that coordinate through a central `Engine` singleton.

**ECS via EnTT** — entities are integer IDs; data lives in components; logic lives in systems. All component types are defined in `Razor/src/Razor/Component.h`:
- `Transform` — position/scale/rotation (Euler + quaternion)
- `Mesh` — reference to an asset by `AssetKey`
- `Camera` / `CameraInfo` — view & projection data
- `DirectionalLight`, `PointLight`, `SpotLight`
- `Collider` — sphere radius for physics
- `ScriptComponent` — holds C# instance handles

The `Coordinator` (singleton) owns the EnTT registry and is the primary way to create entities and add/query components.

**Rendering pipeline** — `Renderer/IRenderer.h` defines the interface; `Platform/OpenGL/OpenGLRenderer` is the only implementation. The pipeline is composed of passes:
- `RSMaterialPass` — material/mesh rendering
- Lighting passes (directional, point, spot)
- `TransformationPass`, `CameraPass`
- `PickBuffer` — for mouse-pick selection in the editor

Framebuffers chain the passes for deferred rendering.

**Physics** — `Physics/JoltPhysics/JoltPhysicsEngine` wraps Jolt. Jolt must be built with RTTI enabled (`-DCPP_RTTI_ENABLED=ON`). The pre-build commands in `premake5.lua` handle this automatically. Physics bodies are driven by `Collider` components and updated by `PhysicsSystem`.

**C# Scripting** — `Scripting/ScriptEngine` manages .NET assembly loading via **Coral** (the `Razor/vendor/Coral` submodule). Scripts attach to entities via `ScriptComponent`. The Coral managed DLL is copied to `Edge/bin/` as a post-build step.

**Scene serialization** — `Scene/SceneSerializer` reads/writes YAML (via yaml-cpp). Scene files live under `Edge/project/` and `Sandbox/assets/`.

**Platform abstraction** — interfaces in `Platform/Generic/` (`IWindowProvider`, `ITimeProvider`, `IPlatformIO`) with OpenGL+GLFW implementations in `Platform/OpenGL/`.

### Editor (`Edge/src/`)

The editor runs its own ImGui UI panels on top of the engine. Editor-specific systems (e.g. scene hierarchy, properties panel, asset browser) live in `Edge/src/Gui/` and `Edge/src/Systems/`. The editor and game runtime run on separate threads.

### Sandbox (`Sandbox/src/`)

Thin application that loads a scene and runs it through the engine. C# game scripts live in `Sandbox/Scripts/`.

## Key Macros & Defines

| Define | Set when |
|---|---|
| `RZ_PLATFORM_WINDOWS` / `RZ_PLATFORM_LINUX` | Platform guards |
| `RZ_BUILD_DLL` | Compiling the Razor library itself |
| `RAZOR_API` | `__declspec(dllexport/import)` on Windows |
| `RZ_DEBUG` / `RZ_RELEASE` / `RZ_DIST` | Active build configuration |
| `JPH_DEBUG_RENDERER` | Enables Jolt physics debug draw |

## Creating a Pull Request

Follow this workflow when committing changes and opening a PR.

**1. Create a branch from `stable`:**
```sh
git checkout stable
git checkout -b <type>-<short-description>   # e.g. fix-linux-csharp-build
```

**2. Stage and commit — use separate commits for distinct concerns:**
```sh
git add <specific files only — never git add -A>
git commit -m "type [Category]: Short description"
```

Commit message format: `type [Category]: Description` — e.g. `fix [Linux]: Fix ImGui editor window docking on Wayland`. Types: `fix`, `feature`, `add`. Category is optional but preferred for scoped changes (e.g. `[Build]`, `[Editor]`, `[Linux]`).

Files to always exclude from commits:
- `Edge/bin/` — binary output
- `Razor/vendor/*` — only exclude if the change is untracked build-generated content inside the submodule (shown as `modified (untracked content)` in `git status`); include it if the submodule has been intentionally updated to a new commit
- `*.make` — Premake-generated Makefiles
- Unrelated scene files (`*.rzscn`)

**3. Push and open a PR targeting `stable`:**
```sh
git push -u origin <branch-name>
gh pr create --base stable --head <branch-name> --title "..." --body "..."
```

**PR description format:**
```
## Description ##
One or two sentences summarising what the PR fixes or adds and why.

## Changes ##
**Category (if grouping makes sense)**
- Specific change with enough detail to understand what and why
- Another change
```

See PRs #86, #81, #79 on GitHub for examples.

## Linux Notes

Linux support is established on this repo. GLFW defaults to the X11 backend (pass `--display-backend=wayland` to `GenerateProjects.sh` to switch). Assimp and JoltPhysics are built from source via CMake pre-build commands. Some Windows-only utilities are excluded via `removefiles` in `premake5.lua`.
