# Collision Detection — Shaping Notes

## Scope

Add a `CollisionComponent` that accumulates Jolt physics collision events (Started, Ended) into a per-frame queue. C++ systems and C# scripts read from this queue each frame. Triggered by issue #84 — the engine fires a fire-and-forget C++ lambda on collision but has no persistent, queryable collision state.

## Decisions

- `CollisionSystem` replaces the old radius-based `Collider` detection and becomes the sole owner of collision event population. `PhysicsSystem` is simplified to position sync only.
- Body-to-entity reverse mapping is built each frame by iterating all `BoxBody` entities — no changes to Jolt body creation or `IPhysicsEngine` interface.
- `CollisionComponent.Events` is cleared at the start of each frame (before new contacts are written). Events live for exactly one frame so all systems in that frame see the same data.
- `OtherEntityId` in `CollisionEvent` is `UINT32_MAX` if the other body has no matching entity (e.g., a static world body with no ECS entity).
- C# API is index-based (`GetEventCount` + `GetEvent(index)`) to avoid cross-boundary struct marshaling complexity.
- `TryGetComponent<T>` is added to the `Scene` API so systems don't access `registry` directly.
- `OnCollisionStarted` lambda on `BoxBody` is preserved and still fired by `CollisionSystem` for backward compat.

## Context

- **Visuals:** None — behaviour-only feature
- **References:** `PhysicsSystem.cpp`, `BoxBody.h`, `IPhysicsEngine.h`, `MyContactListener` in `JoltPhysicsEngine`, `Transform_GetPosition` pattern in `ScriptGlue.cpp`
- **Product alignment:** Directly enables Pong milestone — ball-paddle collision is the primary use case described in the issue

## Standards Applied

- `ecs/smart-pointer-aliases` — `Ref<T>` throughout
- `serialization/scene-serializer-components` — 4-file checklist
- `serialization/yaml-pattern` — YamlWrapper only
- `editor/component-drawers` — `RegisterDrawers()` map
