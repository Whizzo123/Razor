# Collision Detection — Plan

## Problem

During play mode, collision contacts from Jolt are fire-and-forget — a C++ lambda fires and nothing persists. C# scripts have no way to query collision state. Issue #84.

## Solution

Add `CollisionComponent` as a per-frame event queue. `CollisionSystem` (replacing old radius-based code) reads Jolt contacts, clears and repopulates `CollisionComponent.Events` each frame. Two new ScriptGlue internal calls expose the queue to C# scripts.

## Tasks

### Task 1: `Scene.h` — Add `TryGetComponent<T>`
Return `T*` (null if absent) so systems can optionally check for a component without going through `Entity::HasComponent` or touching `registry` directly.

### Task 2: `Component.h` — Add types
```
CollisionEventType { Started, Ended }
CollisionEvent { Type, OtherEntityId }
CollisionComponent { vector<CollisionEvent> Events }
```

### Task 3: Replace `CollisionSystem`
Remove radius/Collider logic. New implementation:
1. Build `bodyId → entityId` map from all `BoxBody` entities
2. For each `BoxBody` entity: clear `CollisionComponent.Events` (if present), get Jolt contacts, push Started/Ended events, fire `OnCollisionStarted` lambda for Started

### Task 4: Simplify `PhysicsSystem`
Remove contact handling (moved to CollisionSystem). Keep only: `GetPosition(bodyId)` → `transform.Position`.

### Task 5: `ScriptGlue.cpp` — Two internal calls
- `Collision_GetEventCount(entityId) -> int`
- `Collision_GetEvent(entityId, index, outType*, outOtherId*) -> void`

### Task 6: C# — `Collision.cs` (new)
`CollisionEventType` enum, `CollisionEvent` struct, `Collision` static API class.

### Task 7: C# — `InternalCalls.cs`
Add delegate pointers and wrappers for the two new calls.

### Task 8: Serialization
Empty serialize/deserialize block for `CollisionComponent` in `SceneSerializer.cpp` — no runtime fields to persist.

### Task 9: Editor
`ComponentImGui` drawer (no configurable properties) + Inspector add-component button.

### Task 10: `ScriptBridge.csproj`
Add `Collision.cs` to explicit `<ItemGroup>`.

## Verification
1. Build: 0 errors
2. Run Edge: no crash
3. Entity with BoxBody + CollisionComponent falls onto static BoxBody entity
4. C# script reads `Collision.GetEventCount` → logs `Started` event
5. Next frame with no new contact: event count is 0
