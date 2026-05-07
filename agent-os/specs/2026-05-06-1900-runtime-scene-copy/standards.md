# Standards for Runtime Scene Copy

## ecs/smart-pointer-aliases

Razor defines aliases for `std::shared_ptr` and `std::unique_ptr`. Always use the aliases and their factory functions — never use `std::make_shared`, `std::make_unique`, or `new` directly.

| Alias | Underlying type | Factory |
|---|---|---|
| `Ref<T>` | `std::shared_ptr<T>` | `CreateRef<T>(args...)` |
| `Scope<T>` | `std::unique_ptr<T>` | `CreateScope<T>(args...)` |

Use `CreateRef<Scene>(path)` when constructing the clone inside `Scene::Clone()`.

## ecs/mandatory-components

`Scene::CreateEntity` always adds `Transform` and `ScriptComponent`. The clone must include both in its per-component copy loop. Do not call `CreateEntity()` when cloning — create raw registry entities and emplace components directly to avoid double-adding mandatory components.

## ecs/entity-wrapper

`Entity` is a disposable proxy; use `entt::entity` handles directly when iterating in systems and in `Scene::Clone()`. The entity map in `Clone()` should be `std::unordered_map<entt::entity, entt::entity>`.
