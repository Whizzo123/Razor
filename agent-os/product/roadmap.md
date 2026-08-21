# Product Roadmap

## Phase 1: MVP

Core systems that make Razor usable for building strategy/simulation games:

- **Visual scene editor (Edge)** — scene hierarchy, entity inspector, asset browser, project management
- **ECS with core components** — Transform, Mesh, Camera, Lights, Collider, and ScriptComponent working reliably via EnTT
- **C# scripting** — game logic attached to entities through the Coral .NET bridge
- **Physics simulation** — Jolt-based rigid body physics with collider components

## Phase 2: Post-Launch

Features planned after the MVP is stable:

- **AI & pathfinding** — navigation mesh generation and agent steering systems suited to simulation entities
- **Rendering improvements** — shadows, post-processing effects, particle systems, material editor enhancements
- **Editor tooling improvements** — better workflow, more panels, undo/redo, improved scene manipulation
- **Asset importing** — robust pipeline for importing meshes, textures, and other assets into projects
