# Standards for CMake Build System Migration

No existing standards in `agent-os/standards/` apply directly to build system work (current standards cover ECS patterns, editor UI lifecycle, rendering passes, and serialization).

## Applied Best Practices

These modern CMake conventions guide this implementation:

- **Target-based CMake**: All properties (includes, defines, links) are set on targets via `target_*` commands, not globally.
- **Generator expressions** for per-config and per-platform behavior rather than `if(WIN32)` blocks mixed into target properties.
- **`PRIVATE`/`PUBLIC`/`INTERFACE` visibility**: include dirs and link libs correctly scoped so consumers don't inherit unnecessary dependencies.
- **No `file(GLOB_RECURSE)` for src/ trees that grow frequently**: Prefer explicit source lists or at minimum document that CMake reconfigure is needed after adding files.
