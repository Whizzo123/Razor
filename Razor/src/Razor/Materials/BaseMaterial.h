#pragma once
#include "Material.h"
#include "../Renderer/Shader.h"
class BaseMaterial :
    public Material
{
public:
    BaseMaterial(std::string VertexShader, std::string FragmentShader, std::vector<std::string>& TextureNames);
    ~BaseMaterial();

    void PassValues(Transform ObjTransform) override;
    
protected:
    Shader Shader;
};

