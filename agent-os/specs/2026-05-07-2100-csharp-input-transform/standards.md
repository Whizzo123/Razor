# Standards for C# Input & Transform

## ecs/smart-pointer-aliases

Razor defines aliases for `std::shared_ptr` and `std::unique_ptr`. Always use the aliases and their factory functions — never use `std::make_shared`, `std::make_unique`, or `new` directly.

| Alias | Underlying type | Factory |
|---|---|---|
| `Ref<T>` | `std::shared_ptr<T>` | `CreateRef<T>(args...)` |
| `Scope<T>` | `std::unique_ptr<T>` | `CreateScope<T>(args...)` |

**Application here:** The C++ internal call implementations retrieve entities via `Engine::Get().CurrentScene->GetEntity(...)` which returns a `Ref<Entity>`. Assign to `Ref<Entity>`, never to a raw pointer.

## ecs/entity-wrapper

`Entity` is a short-lived proxy around an `entt::entity` handle. It is not a persistent identity object.

- Do not store `Ref<Entity>` between frames
- Real identity is the `entt::entity` handle
- `Scene::GetEntity()` returns `nullptr` if the entity has no `Transform` component

**Application here:** Script glue uses the `Ref<Entity>` wrapper (not raw registry access) when looking up entities by `uint32_t` ID — this is the correct pattern per the "editor UI, serialization, script glue" row in this standard. Always null-check the result before accessing components.
