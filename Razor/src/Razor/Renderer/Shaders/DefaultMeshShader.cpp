#include "DefaultMeshShader.h"

namespace Razor
{

    void DefaultMeshShader::Setup(CameraInfo* CamInfo, glm::mat4 model, MaterialData Mat, std::shared_ptr<std::vector<Light>> Light)
    {
        SetMat4Float("projection", CamInfo->CameraProjection);
        SetMat4Float("view", CamInfo->CameraView);
        SetMat4Float("model", model);
        SetInt("material.texture_diffuse", 0);
        SetInt("material.texture_specular", 0);
        SetInt("material.texture_normal", 0);
        SetInt("material.texture_height", 0);
        SetVec3("material.diffuse", Mat.Diffuse);
        SetVec3("material.ambient", Mat.Ambient);
        SetVec3("material.specular", Mat.Specular);
        SetFloat("material.shininess", Mat.Shininess);
        SetBool("material.bUseTextures", 0);
        // Light  ->
        SetVec3("light.position", Light->at(0).Position);
        SetVec3("light.ambient", Light->at(0).Ambient);
        SetVec3("light.diffuse", Light->at(0).Diffuse);
        SetVec3("light.specular", Light->at(0).Specular);
        // Camera ->
        SetVec3("viewPos", CamInfo->CameraPosition);
    }

}