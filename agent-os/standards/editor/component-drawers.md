# Component Drawers

The Inspector uses `ComponentImGui` to draw per-component UI. Drawers are registered as a static map in `ComponentImGui::RegisterDrawers()` (ComponentImGui.cpp).

## Adding a drawer for a new component

1. Write a static draw function:
```cpp
static void DrawMyComponent(Razor::Ref<Razor::Entity> InEntity)
{
    if (!InEntity->HasComponent<Razor::MyComponent>()) return;  // always check first
    Razor::MyComponent& comp = InEntity->GetComponent<Razor::MyComponent>();
    if (Razor::RazorImGui::CollapsingHeader("My Component")) {
        // ... ImGui widgets
    }
}
```

2. Register it in `RegisterDrawers()`:
```cpp
map["MyComponent"] = DrawMyComponent;  // key = bare C++ class name, no namespace
```

3. Add the component button in `Inspector::Render()` so users can add it to entities.

## Rules

- Always call `HasComponent<T>()` before `GetComponent<T>()` — skipping this crashes
- String key must be the exact C++ class name (e.g. `"Transform"`, not `"Razor::Transform"`)
- If a drawer needs the asset picker popup, access it via `ComponentImGui::SetAssetPickerPopup()`
