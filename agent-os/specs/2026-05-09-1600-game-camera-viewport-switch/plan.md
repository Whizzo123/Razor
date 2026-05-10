# Game Camera Viewport Switch — Plan

## Problem

During play mode, WASD input moves both the editor camera and the game camera simultaneously. Issue #83.

## Solution

Two viewport panels: **Scene** (editor camera) and **Game** (game camera). Hover focus determines which camera receives input. Uses existing `RSCameraPass` render system — no new systems needed.

## Tasks

### Task 1: Register RSCameraPass and build GamePipelineConfig (`Edge/src/EdgeApp.cpp`)
- Register `RSCameraPass(Engine.CurrentScene, Engine.GetRenderer())` in `Edge::Run()`
- Create `GamePipelineConfig` — same as `EditorPipelineConfig` but with `RSCameraPass` at `RENDER_STAGE_CAMERA_PASS`
- Remove/replace the unused `PipelineConfig` variable which incorrectly used `RSEditorCamera`

### Task 2: Add GameBuffer (`Edge/src/EdgeApp.h`, `Edge/src/EdgeApp.cpp`)
- Add `Razor::Ref<Razor::Framebuffer> GameBuffer` member
- Initialise after `SceneBuffer`: `GameBuffer = Engine.Renderer->CreateFrameBuffer(300, 200)`
- In main loop: `GameBuffer->Refresh(SizeX, SizeY)` + `Engine.Render(GameBuffer->GetID(), GamePipelineConfig)`

### Task 3: Add Game ImGui panel (`Edge/src/EdgeApp.h`, `Edge/src/EdgeApp.cpp`)
- Add `void RenderGameViewport(Razor::Ref<Razor::Framebuffer>)` declaration and implementation
- Model on `RenderSceneViewport()` with window title `"Game"`
- Capture `ImGui::IsWindowHovered()` into `bGameViewHovered` inside Game panel
- Capture `ImGui::IsWindowHovered()` into `bSceneViewHovered` inside Scene panel
- Add `bool bSceneViewHovered = false` and `bool bGameViewHovered = false` members

### Task 4: Focus-based input routing (`Edge/src/EdgeApp.cpp`)
- In `ProcessInput()`: call `EditorCamera.ProcessInput()` only when `!bRuntimeRunning || bSceneViewHovered`
- Call `Engine::Get().SetGameInputEnabled(bRuntimeRunning && bGameViewHovered)`

### Task 5: Game input flag on Engine (`Razor/src/Razor/Engine.h`, `CameraController.cpp`)
- Add `std::atomic<bool> bIsGameInputEnabled { true }` — defaults `true` for standalone game
- Add `SetGameInputEnabled(bool)` and `IsGameInputEnabled()` accessors
- In `CameraController::Run()`: early-return if `!Engine::Get().IsGameInputEnabled()`

## Verification
1. `make -j$(nproc) config=debug` — must exit 0
2. Run `./bin/Debug-linux-x86_64/Edge/Edge`
3. Open scene with Camera entity → Press Play
4. Hover Game panel: WASD moves game camera only
5. Hover Scene panel: WASD moves editor camera only
6. Press Stop: editor camera works normally
