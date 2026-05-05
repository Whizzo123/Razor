# YAML Serialization Pattern

All serializers use `YamlWrapper` — never include yaml-cpp headers directly.

## Emit (write to file)

```cpp
YamlEmitter* Out = yaml_emitter_new();
yaml_emitter_begin_map(Out);
yaml_emitter_key(Out, "Name");
yaml_emitter_value_string(Out, value.c_str());
yaml_emitter_key(Out, "Position");
yaml_emitter_value_vec3(Out, transform.Position);
yaml_emitter_end_map(Out);

std::ofstream FOut(path.c_str());
FOut << Razor::yaml_emitter_cstr(Out);
FOut.close();
```

## Decode (read from file)

```cpp
YamlNode* Data = yaml_load_file(path.c_str());

// Validate required top-level keys before proceeding
if (!yaml_get_child(Data, "RequiredKey")) {
    RZ_CORE_ERROR("Missing required key");
    return false;
}

std::string value = yaml_as_string(yaml_get_child(Data, "KeyName"));
glm::vec3 pos = yaml_as_vec3(yaml_get_child(Data, "Position"));
// Cleanup is handled automatically
```

## Rules

- Always validate required top-level keys with `RZ_CORE_ERROR` and early return if missing
- Optional component blocks use `if (yaml_get_child(...))` with no logging on absence
- Use typed `yaml_as_*` accessors: `yaml_as_string`, `yaml_as_int`, `yaml_as_float`, `yaml_as_vec3`
- File cleanup is automatic — do not call `yaml_free_node()` manually
