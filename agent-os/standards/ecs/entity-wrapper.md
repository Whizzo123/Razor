# Entity Wrapper

`Entity` is a short-lived proxy around an `entt::entity` handle. It is not a persistent identity object.

## Rules

- Do not store `Ref<Entity>` between frames — create a new one via `Scene::GetEntity()` each time
- Real entity identity is the `entt::entity` handle (stored as `Entity::EntityHandle`)
- `Scene::GetEntity()` returns `nullptr` if the entity has no `Transform` component

## When to use each form

| Context | Use |
|---|---|
| Editor UI, serialization, script glue | `Ref<Entity>` wrapper |
| Systems (PhysicsSystem, CameraController, etc.) | Raw `entt::entity` handle + `CurrentScene->registry` |

## Systems: direct registry access

```cpp
void MySystem::Run(float dt)
{
    auto view = CurrentScene->GetEntitiesWithComponents<MyComponent>();
    for (auto entity : view)
    {
        MyComponent& comp = CurrentScene->GetComponent<MyComponent>(entity);
        // ... operate on comp
    }
}
```

- Systems never wrap handles in `Entity` objects — this avoids allocation overhead in the hot loop
- `CurrentScene` is the `Ref<Scene>` inherited from the `System` base class
