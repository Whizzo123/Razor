# Mandatory Components

Every entity created via `Scene::CreateEntity()` always receives two components automatically:

- `Transform` — position, scale, rotation
- `ScriptComponent` — holds C# script instance handles

These cannot be omitted. Do not call `AddComponent<Transform>()` or `AddComponent<ScriptComponent>()` manually after `CreateEntity()` — they are already present.

## Creating an entity

```cpp
Razor::Ref<Razor::Entity> entity = CurrentScene->CreateEntity();
// Transform and ScriptComponent are already added
entity->AddComponent<Razor::Mesh>();  // add optional components after
```

## GetEntity() validation

`Scene::GetEntity()` returns `nullptr` if the entity handle has no `Transform`. Since all valid entities have one, a `nullptr` result means the handle is invalid or stale.
