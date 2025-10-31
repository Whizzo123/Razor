#include "DebugLightShader.h"
#include "../../ShaderReader.h"

namespace Razor
{
    DebugLightShader::DebugLightShader() : Shader()
    {
        ID = ShaderReader::CreateShaderProgram("lightvertex.vert", "lightfragment.frag");
    }

    void DebugLightShader::Setup(CameraInfo* CameraInfo, glm::mat4 Model, MaterialData Mat, std::shared_ptr<std::vector<Light*>> Lights)
    {
        SetMat4Float("projection", CameraInfo->CameraProjection);
        SetMat4Float("view", CameraInfo->CameraView);
        SetMat4Float("model", Model);
    }
}
