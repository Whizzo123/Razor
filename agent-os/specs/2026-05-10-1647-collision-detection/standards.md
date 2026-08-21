# Standards for Collision Detection

The following standards apply to this work.

---

## ecs/smart-pointer-aliases

Use `Ref<T>`/`CreateRef<T>` and `Scope<T>`/`CreateScope<T>` — never `make_shared`, `make_unique`, or `new`.

---

## ecs/mandatory-components

`Scene::CreateEntity` always adds `Transform` and `ScriptComponent`. Do not add them manually. New components (like `CollisionComponent`) must be added explicitly by the user via the inspector or scene file.

---

## serialization/scene-serializer-components

Adding a component requires touching four files:

1. `Razor/src/Razor/Component.h` — define the component struct
2. `Razor/src/Razor/Scene/SceneSerializer.cpp` — serialize + deserialize blocks
3. `Edge/src/Gui/ComponentImGui.cpp` — register drawer in `RegisterDrawers()`
4. `Edge/src/Inspector.cpp` — add "Add Component" button

### Serialize block pattern
```cpp
if (InEntity.HasComponent<MyComponent>()) {
    yaml_emitter_key(Out, "MyComponent");
    yaml_emitter_begin_map(Out);
    yaml_emitter_end_map(Out);
}
```

### Deserialize block pattern
```cpp
if (yaml_get_child(EntityNode, "MyComponent"))
    DeserializedEntity->AddComponent<MyComponent>();
```

---

## serialization/yaml-pattern

All serializers use `YamlWrapper` — never include yaml-cpp headers directly.

- Use `yaml_emitter_key`, `yaml_emitter_value_*`, `yaml_emitter_begin_map`, `yaml_emitter_end_map`
- Use `yaml_get_child`, `yaml_as_string`, `yaml_as_float`, `yaml_as_int`, `yaml_as_vec3`
- Optional component blocks use `if (yaml_get_child(...))` with no logging on absence

---

## editor/component-drawers

Register new drawers in `ComponentImGui::RegisterDrawers()`:
```cpp
map["ComponentName"] = DrawComponentName;
```

Drawer signature: `static void DrawX(Razor::Ref<Razor::Entity> InEntity)`

Always guard with `InEntity->HasComponent<T>()` before accessing the component.
