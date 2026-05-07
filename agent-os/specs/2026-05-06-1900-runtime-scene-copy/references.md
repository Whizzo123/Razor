# References for Runtime Scene Copy

## Scene::MoveFrom

- **Location:** `Razor/src/Razor/Scene/Scene.cpp:210–215`
- **Relevance:** Enumerates every field that belongs to a Scene: `registry`, `FilePath`, `mSystemInstanceHandles`. Clone must handle the same set.
- **Key pattern:** Fields are moved, not copied. Clone does the same but by value-copy or `registry` component iteration.

## Engine::LoadProject

- **Location:** `Razor/src/Razor/Engine.cpp:248–256`
- **Relevance:** Established pattern for creating a `Ref<Scene>` and populating it before making it `CurrentScene`.
- **Key pattern:** `CreateRef<Scene>(path)` → populate → assign to `CurrentScene`. Clone follows the same shape.

## Scene::StartScene / StopScene

- **Location:** `Razor/src/Razor/Scene/Scene.cpp:69–208`
- **Relevance:** Shows exactly what state is created at play-start (physics bodies, C# instances) and destroyed at play-stop. The clone is the target of both calls; the editor scene ref must never pass through either.
