# Render System Structure

All render passes are `RS*` classes inheriting from `RenderSystem`. Each system maps to exactly one `RenderStage`.

## Creating a new render system

```cpp
// RSMyPass.h
class RSMyPass : public Razor::RenderSystem
{
public:
    RSMyPass(Razor::Ref<Razor::Scene> Scene, std::shared_ptr<Razor::IRenderer> Renderer)
        : RenderSystem(Scene), Renderer(Renderer)
    {
        SystemRenderStage = Razor::RenderStage::RENDER_STAGE_MATERIAL_PASS;  // pick one stage
    }
    void Render(Razor::RenderPipelineData& data) override;
private:
    std::shared_ptr<Razor::IRenderer> Renderer;
};
```

- `SystemRenderStage` must be set in the constructor — this is how `RegisterSystem()` places the system into the correct pipeline bucket
- Override only `Render(RenderPipelineData& data)` — no other methods
- Name files `RS{Name}.h` / `RS{Name}.cpp` under `Razor/src/Razor/Systems/`
- Each class maps to exactly one `RenderStage`; a system cannot span multiple stages

## Registering the system

Call `RegisterSystem()` in the application layer (e.g. `EdgeApp.cpp`):
```cpp
Coordinator::Get().GetSystemManager().RegisterSystem(RSMyPass(Scene, Renderer));
```

Then add it to the `RenderPipelineConfig` — see the Runtime Pipeline Config standard.
