# Smart Pointer Aliases

Razor defines aliases for `std::shared_ptr` and `std::unique_ptr`. Always use the aliases and their factory functions — never use `std::make_shared`, `std::make_unique`, or `new` directly.

| Alias | Underlying type | Factory |
|---|---|---|
| `Ref<T>` | `std::shared_ptr<T>` | `CreateRef<T>(args...)` |
| `Scope<T>` | `std::unique_ptr<T>` | `CreateScope<T>(args...)` |

## Usage

```cpp
// Shared ownership
Razor::Ref<Razor::Scene> Scene = Razor::CreateRef<Razor::Scene>();

// Exclusive ownership
Razor::Scope<MySystem> System = Razor::CreateScope<MySystem>(Scene);
```

## Rules

- Use `Ref<T>` when multiple owners need the object (scenes, systems, editor views)
- Use `Scope<T>` when one owner is clear and transfer isn't needed
- Using `std::make_shared<T>`, `std::make_unique<T>`, or raw `new` is a style violation — change it to the alias form
