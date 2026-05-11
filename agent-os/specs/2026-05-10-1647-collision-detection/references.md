# References for Collision Detection

## Similar Implementations

### PhysicsSystem — existing contact retrieval pattern

- **Location:** `Razor/src/Razor/Systems/PhysicsSystem.cpp`
- **Relevance:** Shows how to call `Engine::Get().GetPhysicsEngine().GetContactInfo(body.bodyId)` and iterate `ContactInfo` results — this pattern moves into `CollisionSystem`
- **Key patterns:** Iterate `GetEntitiesWithComponents<BoxBody, Transform>()`, retrieve `ContactInfo`, check `EContactType`

### MyContactListener — Jolt contact storage

- **Location:** `Razor/src/Razor/Physics/JoltPhysics/JoltPhysicsEngine.h` / `.cpp`
- **Relevance:** Stores Started/Persisted/Ended contacts in `_mBodyContactMap` keyed by `JPH::BodyID`; `GetContactInfo(bodyId)` clears Ended contacts on read
- **Key patterns:** Thread-safe via `std::scoped_lock`; `mOtherBodyId` is the other body's Jolt ID

### Transform_GetPosition / Transform_SetPosition — ScriptGlue pattern

- **Location:** `Razor/src/Razor/Scripting/ScriptGlue.cpp` lines 111–123
- **Relevance:** Reference for the `Collision_GetEventCount` / `Collision_GetEvent` internal call pattern — entity lookup, null guard, out-pointer pattern
- **Key patterns:** `Engine::Get().CurrentScene->GetEntity(static_cast<entt::entity>(entityId))`, null check, write to `float*` out params

### DrawCamera — ComponentImGui drawer pattern

- **Location:** `Edge/src/Gui/ComponentImGui.cpp` lines 224–234
- **Relevance:** Minimal drawer for a component with no configurable properties — `DrawCollisionComponent` follows the same structure
