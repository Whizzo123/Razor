# Runtime Pipeline Config

Render execution order is defined by a `RenderPipelineConfig` vector constructed in `EdgeApp.cpp`. The engine has no hardcoded render order.

## Structure

```cpp
Razor::RenderPipelineConfig PipelineConfig = {
    { Razor::RenderStage::RENDER_STAGE_MATERIAL_PASS,  { typeid(RSMaterialPass).name() } },
    { Razor::RenderStage::RENDER_STAGE_CAMERA_PASS,    { typeid(RSCameraPass).name() } },
    { Razor::RenderStage::RENDER_STAGE_RENDER,         { typeid(RSRenderPass).name() } },
};
```

## Adding a new pass to the pipeline

1. Create and register the system (see Render System Structure standard)
2. Add an entry to `PipelineConfig` in `EdgeApp.cpp` at the correct position
3. The system must be registered via `RegisterSystem()` **before** the config is run

## Rules

- `typeid(T).name()` is the only valid key — do not use string literals or class name strings
- Order of entries in the vector = execution order
- Each stage can list multiple systems; they run in the order listed
- A system not in the config is never called, even if registered
