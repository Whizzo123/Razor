# C# Input & Transform — Shaping Notes

## Scope

C# game scripts have no way to read input or modify entity transforms. We are adding:
1. `Input.GetKey(RazorKey key)` → `RazorKeyState` — wraps `RazorIO::Get().GetStateForKey()`
2. `Transform.GetPosition(uint entityId)` → `(float X, float Y, float Z)` — reads `Transform::Position` from the EnTT registry
3. `Transform.SetPosition(uint entityId, float x, float y, float z)` — writes `Transform::Position`

This is a pure addition to the scripting bridge — no engine or editor code changes.

## Decisions

- **Three separate float out-params** for GetPosition rather than a struct pointer — avoids C++/C# struct-by-value ABI concerns across `extern "C"` boundary; cleaner for a first pass.
- **Static helper classes** (`Input`, `Transform`) as the public C# API — consistent with how `Log.cs` wraps `InternalCalls.LogMsg`. Script authors never call `InternalCalls` directly.
- **`IsKeyPressed` convenience method** on `Input` — any non-Released state counts as "held or pressed", which is the common check for movement scripts.
- **Tuple return** for `GetPosition` — avoids defining a new Vector3 struct for now; extend to a proper struct later when rotation/scale are added.
- **Enum values must match C++** — `RazorKey` and `RazorKeyState` integer values in C# are hardcoded to match the C++ enum order exactly.

## Context

- **Visuals:** None
- **References:** `ScriptGlue.cpp` + `InternalCalls.cs` — existing internal call pattern end-to-end. `Log.cs` — example of a typed wrapper over `InternalCalls`.
- **Product alignment:** Direct MVP requirement. The MVP goal is "C# scripting — game logic attached to entities through the Coral .NET bridge." This feature makes movement scripts possible for the first time.

## Standards Applied

- `ecs/smart-pointer-aliases` — `Ref<Entity>` used in `Transform_GetPosition`/`Transform_SetPosition` C++ implementations
- `ecs/entity-wrapper` — Script glue uses `Ref<Entity>` (not raw `entt::entity`) to look up entities by ID, per the "editor UI, serialization, script glue" row in that standard
