# Product Mission

## Problem

Building strategy and simulation games requires an engine that can efficiently handle large numbers of active entities without the overhead and complexity of general-purpose commercial engines. Existing options (Unity, Unreal, Godot) are either too heavy, too opaque, or not designed with ECS-first thinking that suits simulation workloads.

## Target Users

Hobbyist and indie developers who want a lightweight, approachable engine for strategy and simulation games. Also open-source contributors and learners who want to understand engine internals by reading and modifying real, working code.

## Solution

Razor is a lightweight, hackable 3D game engine built from scratch in C++20. It is:

- **ECS-first** — built around EnTT from the ground up, making it naturally suited to simulations with hundreds or thousands of entities
- **Transparent** — a small, understandable codebase where every layer is accessible and modifiable
- **Complete enough to ship** — includes a visual scene editor (Edge), C# scripting via Coral, Jolt physics, and OpenGL rendering out of the box

The goal is not the best graphics or the fastest iteration time — it is an engine you can fully understand, extend, and learn from.
