#include "DebugLightShader.h"

void Razor::DebugLightShader::Setup(CameraInfo* CameraInfo, glm::mat4 Model, MaterialData Mat, std::shared_ptr<std::vector<Light>> Lights)
{
    SetMat4Float("projection", CameraInfo->CameraProjection);
    SetMat4Float("view", CameraInfo->CameraView);
    SetMat4Float("model", Model);
}
