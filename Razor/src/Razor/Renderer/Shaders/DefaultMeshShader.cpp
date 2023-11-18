#include "DefaultMeshShader.h"

namespace Razor
{

    void DefaultMeshShader::Setup(CameraInfo* CamInfo, glm::mat4 model, MaterialData Mat, std::shared_ptr<std::vector<Light*>> Lights)
    {
        SetMat4Float("projection", CamInfo->CameraProjection);
        SetMat4Float("view", CamInfo->CameraView);
        SetMat4Float("model", model);
       
        // Light  ->
        SetVec3("light.position", Lights->at(0)->Position);
        SetVec3("light.ambient", Lights->at(0)->Ambient);
        SetVec3("light.diffuse", Lights->at(0)->Diffuse);
        SetVec3("light.specular", Lights->at(0)->Specular);
        const char* TypeName = typeid(Lights->at(0)).name();
        if (TypeName == typeid(DirectionalLight).name())
        {
            std::shared_ptr<Light> LightPtr = std::shared_ptr<Light>(Lights->at(0));
            std::shared_ptr<DirectionalLight> DirLight = std::dynamic_pointer_cast<DirectionalLight>(LightPtr);
            if (DirLight != nullptr)
            {
                SetVec3("light.direction", DirLight->Direction);
            }
            else
            {
                RZ_ERROR("Failed to convert light to directional light even though passed type check ;(");
            }
        }
        // Camera ->
        SetVec3("viewPos", CamInfo->CameraPosition);
    }

}