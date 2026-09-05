#include "DebugLightShader.h"
#include "../../ShaderReader.h"
#include "../../Log.h"

namespace Razor
{
    DebugLightShader::DebugLightShader() : Shader()
    {
        Scope<unsigned int> program = ShaderReader::CreateShaderProgram("lightvertex.vert", "lightfragment.frag");

        if (!program) {
			ID = 0;
			RZ_CORE_ERROR("PickBufferShader failed to create shader program will default to shader in slot 0");
			return;
		}
		ID = *program;
    }

    void DebugLightShader::Setup(CameraInfo* CameraInfo, glm::mat4 Model, [[maybe_unused]]MaterialData Mat, [[maybe_unused]]std::shared_ptr<std::vector<Light*>> Lights)
    {
        SetMat4Float("projection", CameraInfo->CameraProjection);
        SetMat4Float("view", CameraInfo->CameraView);
        SetMat4Float("model", Model);
    }
}
