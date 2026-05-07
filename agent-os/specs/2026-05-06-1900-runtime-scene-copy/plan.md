# Runtime Scene Copy

## Context

The editor currently runs the simulation on the live `CurrentScene`. Physics mutates `Transform.Position`, C# scripts can mutate component fields, and stopping the runtime leaves the scene dirty. The fix is to deep-clone the scene before play, run on the clone, and discard it on stop — restoring the original editor scene ref unchanged.

## Approach

Add `Scene::Clone()` which builds a fresh `entt::registry` by iterating all component types and emplacing copies, then update `Engine::RuntimeStart` / `RuntimeStop` to swap `CurrentScene` with the clone.

---

## Task 1: Save Spec Documentation ✓

`agent-os/specs/2026-05-06-1900-runtime-scene-copy/` created with plan.md, shape.md, standards.md, references.md.

---

## Task 2: Add `Scene::Clone()`

**Files:** `Razor/src/Razor/Scene/Scene.h`, `Razor/src/Razor/Scene/Scene.cpp`

Add public `Ref<Scene> Clone() const;` to Scene.h.

In Scene.cpp, add a file-local template helper and implement Clone():
- Create a new Scene with the same FilePath
- Copy mSystemInstanceHandles (vector copy)
- Build entity map: `registry.each()` → `copy->registry.create()`
- Emplace each component type using the map

Components to copy: `Transform`, `ScriptComponent`, `Mesh`, `Camera`, `CameraInfo`, `DirectionalLight`, `PointLight`, `SpotLight`, `Collider`, `BoxBody`, `Input`.

Do NOT use `CreateEntity()` — it double-adds mandatory components.

---

## Task 3: Update Engine RuntimeStart / RuntimeStop

**Files:** `Razor/src/Razor/Engine.h`, `Razor/src/Razor/Engine.cpp`

Add `Ref<Scene> _mEditorScene;` to private section of Engine.h.

RuntimeStart: save `CurrentScene` to `_mEditorScene`, assign `CurrentScene->Clone()` to `CurrentScene`, then proceed with StartScene + thread spawn as before.

RuntimeStop: after joining thread and calling StopScene, restore `CurrentScene = _mEditorScene` and reset `_mEditorScene = nullptr`.

---

## Verification

1. Build with MSBuild — 0 errors expected.
2. Run Edge, load a scene with physics/script entities.
3. Note entity positions. Press Play, observe mutation. Press Stop.
4. Confirm positions match pre-play values.
5. Repeat Play/Stop to verify second cycle works correctly.
