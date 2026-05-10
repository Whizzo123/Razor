# Game Camera Viewport Switch — Shaping Notes

## Scope

Add a **Game** panel to the Edge editor that renders from the active game camera (`RSCameraPass`) during play mode. Hover focus routes input: hovering **Scene** gives WASD to the editor camera; hovering **Game** gives WASD to the game. Neither panel gets input when the other is hovered. Mirrors the Unity/Godot editor model.

Triggered by issue #83 — WASD during play moved both the editor camera and the player character simultaneously.

## Decisions

- Input routing is hover-based (not click-to-focus) — matches how the existing pick-object logic works in the Scene panel
- `bIsGameInputEnabled` defaults to `true` so the standalone game executable works without an editor to set the flag
- `RSCameraPass` already exists in `Razor/src/Razor/Systems/` — no new render system needed, just registration and pipeline wiring
- The unused `PipelineConfig` in `EdgeApp.cpp` (lines 71–81) was clearly intended as the game pipeline but incorrectly used `RSEditorCamera` — will be replaced with the corrected `GamePipelineConfig`
- Game input flag lives on `Engine` as `std::atomic<bool>` matching the existing `bIsRuntimeRunning` pattern

## Context

- **Visuals:** None provided — behaviour is clear from issue description and Unity/Godot reference
- **References:** `RSEditorCamera`, `RSCameraPass`, `RenderSceneViewport()`, `Engine::bIsRuntimeRunning`
- **Product alignment:** Editor polish directly supports MS1 milestone goal of a working Pong clone — scripted gameplay needs reliable input isolation during play

## Standards Applied

- `rendering/runtime-pipeline-config` — used to define `GamePipelineConfig` with `RSCameraPass` at camera pass stage
- `rendering/render-system-structure` — used to register `RSCameraPass` in `EdgeApp.cpp`
