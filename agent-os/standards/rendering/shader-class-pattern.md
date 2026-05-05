# Shader Class Pattern

Shader classes are thin wrappers that load GLSL source files at startup. No business logic belongs in a shader class.

## Creating a new shader

```cpp
// MyShader.h
class MyShader : public Razor::Shader
{
public:
    MyShader();
};

// MyShader.cpp
MyShader::MyShader() : Shader()
{
    ID = Razor::ShaderReader::CreateShaderProgram("myshader.vert", "myshader.frag");
}
```

## Shader source files

- Place `.vert` and `.frag` files in `resources/shaders/` inside the currently opened project directory
- `ShaderReader` loads from `resources/shaders/<filename>` relative to the project root
- File names are passed as-is to `CreateShaderProgram` — no path prefix needed

## Rules

- Constructor sets `ID` only — do not add state or methods to shader classes
- Use the base `Shader` methods to set uniforms: `SetFloat()`, `SetInt()`, `SetVec3()`, `SetMat4Float()`, `SetBool()`
- Shader is instantiated once and reused; `CreateShaderProgram` compiles at construction time
