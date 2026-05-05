# Scene Serializer & New Components

The scene serializer has no reflection — every component type is hardcoded with explicit serialize and deserialize blocks. Adding a new component requires touching four files.

## Checklist for a new component type

1. **`Razor/src/Razor/Component.h`** — define the component struct
2. **`Razor/src/Razor/IO/SceneSerializer.cpp`** — add serialize block in `SerializeEntity()` and deserialize block in `DeserializeEntity()`
3. **`Edge/src/Gui/ComponentImGui.cpp`** — register a drawer in `RegisterDrawers()` (see Component Drawers standard)
4. **`Edge/src/Inspector.cpp`** — add an "Add Component" button in `Render()`

## Serialize block pattern

```cpp
if (InEntity.HasComponent<MyComponent>()) {
    MyComponent& comp = InEntity.GetComponent<MyComponent>();
    yaml_emitter_key(Out, "MyComponent");
    yaml_emitter_begin_map(Out);
    yaml_emitter_key(Out, "Value");
    yaml_emitter_value_float(Out, comp.Value);
    yaml_emitter_end_map(Out);
}
```

## Deserialize block pattern

```cpp
auto MyComponentNode = yaml_get_child(EntityNode, "MyComponent");
if (MyComponentNode) {
    MyComponent& comp = entity->AddComponent<MyComponent>();
    comp.Value = yaml_as_float(yaml_get_child(MyComponentNode, "Value"));
}
```

- Missing component nodes are silently skipped (no log) — this is intentional for forward compatibility
- Always use `HasComponent<T>()` before `GetComponent<T>()` in the serialize block
