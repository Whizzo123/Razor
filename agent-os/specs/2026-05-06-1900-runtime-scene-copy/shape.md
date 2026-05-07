# Runtime Scene Copy — Shaping Notes

## Scope

When the editor starts play mode, the simulation currently runs directly on the live editor scene. Physics updates entity transforms, C# scripts mutate component data, and when the user stops the runtime those mutations persist in the editor — the scene is dirty.

We want to:
1. Deep-copy the editor scene immediately before play starts
2. Run the simulation on the copy
3. On stop, destroy the copy and restore `CurrentScene` to the original editor scene ref

## Decisions

- **Copy mechanism:** Deep copy via `Scene::Clone()` — builds an entity map and copies each component type manually. Chosen over the YAML serialize/deserialize approach because it avoids YAML parsing overhead, keeps all logic in C++, and is more semantically clear.
- **Restore behavior:** Full restore to pre-play state. Runtime mutations are discarded on stop.
- **No UI change needed:** Play/Stop buttons in `Edge::CreateDockspace()` already call `Engine::RuntimeStart()` / `Engine::RuntimeStop()` — all logic stays inside the Engine.
- **Pointer swap:** We save the original `CurrentScene` shared_ptr as `_mEditorScene` and assign the cloned `Ref<Scene>` to `CurrentScene`. The original scene object is never touched during play.
- **`ScriptComponent` handle sharing is safe:** `mScriptInstances` contains `uint64_t` handles into `ScriptInterface::ScriptInstancePool`. The only field mutated at play-time is `ScriptInstance::handle` (the C# object ID), which is re-set on every `StartScene()` call. `className` and `fields` are read-only during play.
- **`BoxBody::bodyId` is pre-play clean:** Default value is `0xFFFFFFFF`, only set during `StartScene()`. A copy made before play has correct defaults.

## Context

- **Visuals:** None
- **References:** `Engine::LoadProject()` (`Engine.cpp:248–256`) — shows the established pattern for creating a `Ref<Scene>` and deserializing into it before assigning to `CurrentScene`. `Scene::MoveFrom()` (`Scene.cpp:210–215`) — shows all fields that need to be handled: `registry`, `FilePath`, `mSystemInstanceHandles`.
- **Product alignment:** Direct MVP requirement — Edge is not usable as a game editor without play-mode isolation

## Standards Applied

- `ecs/smart-pointer-aliases` — Use `CreateRef<Scene>` when constructing the clone
- `ecs/mandatory-components` — Every entity has `Transform` and `ScriptComponent`; clone must cover both
