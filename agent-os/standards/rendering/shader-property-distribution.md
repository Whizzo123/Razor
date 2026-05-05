# Shader Property Distribution

Each RS* pass adds typed properties to `RenderPipelineData`. `RSRenderPass` consumes them all at the end of the frame and clears them per-entity.

## Adding properties in a pass

```cpp
void RSMyPass::Render(RenderPipelineData& data)
{
    for (auto Entity : data.mEntities)
    {
        ShaderPropertySlot& Slot = data.mEntityRenderProperties
                                       .Properties[Entity].GetPropertySlot(0);
        Slot.AddProperty<glm::mat4>("myUniform", value);  // name must match GLSL uniform
        Slot.AddProperty<float>("myFloat", 1.0f);
    }
}
```

## Rules

- Property name strings are GLSL uniform names — `RSMaterialPass.cpp` is the canonical reference for valid names
- Properties are **cleared after each entity renders** — re-add them every frame
- Duplicate property names for the same entity cause a **silent overwrite** — last `AddProperty` wins
- `RSRenderPass` dispatches properties by type via `dynamic_cast` — only `float`, `int`, `bool`, `glm::vec3`, `glm::mat4` are handled

## Supported property types

| C++ type | Shader setter called |
|---|---|
| `float` | `shader->SetFloat()` |
| `int` | `shader->SetInt()` |
| `bool` | `shader->SetBool()` |
| `glm::vec3` | `shader->SetVec3()` |
| `glm::mat4` | `shader->SetMat4Float()` |

Adding an unsupported type compiles but is silently ignored at render time.
