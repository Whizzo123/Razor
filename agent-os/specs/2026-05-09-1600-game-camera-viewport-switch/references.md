# References for Game Camera Viewport Switch

## Similar Implementations

### RSEditorCamera

- **Location:** `Edge/src/Systems/RSEditorCamera.h` / `RSEditorCamera.cpp`
- **Relevance:** The existing editor camera render system — shows how a camera render system is registered in `EdgeApp.cpp` and how it populates `view`, `projection`, `viewPos` shader properties
- **Key patterns:** Constructor takes `(Ref<Scene>, shared_ptr<IRenderer>, Camera&)`; registered via `Engine.GetCoordinator()->RegisterSystem<RSEditorCamera>(...)`

### RSCameraPass

- **Location:** `Razor/src/Razor/Systems/RSCameraPass.h` / `RSCameraPass.cpp`
- **Relevance:** The game camera render system to be wired into the `GamePipelineConfig` — queries all entities with a `Camera` component and applies the first found to the pipeline
- **Key patterns:** Constructor takes `(Ref<Scene>, shared_ptr<IRenderer>)`; identical shader property population to `RSEditorCamera`

### RenderSceneViewport

- **Location:** `Edge/src/EdgeApp.cpp:176–184`
- **Relevance:** The existing Scene ImGui panel — `RenderGameViewport()` is modelled directly on this function
- **Key patterns:** `ImGui::Begin("Scene")`, `ImGui::Image(framebuffer texture)`, `ImGui::GetItemRectMin()` for viewport offset

### Engine::bIsRuntimeRunning

- **Location:** `Razor/src/Razor/Engine.h:204`
- **Relevance:** Pattern for the new `bIsGameInputEnabled` flag — `std::atomic<bool>` with getter, set from the editor layer (EdgeApp), read from the game thread (CameraController)

### CameraController

- **Location:** `Razor/src/Razor/Systems/CameraController.h` / `CameraController.cpp`
- **Relevance:** Game-side WASD input processing — receives the `IsGameInputEnabled()` guard
