# AssetKey

`AssetKey` is a type-safe wrapper around an asset's relative file path. It is the standard way to reference assets in components and serialized scenes.

## How keys are created

Valid `AssetKey` values come from two sources only:

1. **`AssetDirectory`** — the runtime asset registry; call into it to resolve assets by path
2. **Scene deserialization** — `SceneSerializer` reconstructs keys by reading the saved path string from the `.rzscn` file

Do not construct `AssetKey` manually from arbitrary strings anywhere else.

## Key format

The key is the asset's file path relative to the project root (e.g. `models/cube.model`).

## Usage patterns

```cpp
// Check if an entity has a valid asset assigned
if (!mesh.mKey) {  // bool operator: true if key is non-empty
    return;
}

// Use as map key
std::unordered_map<Razor::AssetKey, Razor::Model> Cache;

// Get the raw path string (e.g. for serialization)
std::string path = mesh.mKey.GetKey();
```

## Serialization

When serializing a `Mesh` component, write the key string:
```cpp
yaml_emitter_value_string(Out, EntityMesh.mKey.GetKey().c_str());
```

When deserializing, reconstruct from the saved string:
```cpp
AssetKey key(yaml_as_string(yaml_get_child(MeshNode, "MeshKey")));
```
