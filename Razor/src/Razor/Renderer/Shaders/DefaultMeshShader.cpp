#include "DefaultMeshShader.h"
#include "../../ShaderReader.h"
#include "../../Log.h"

namespace Razor
{

    DefaultMeshShader::DefaultMeshShader() : Shader()
    {
        Scope<unsigned int> program = ShaderReader::CreateShaderProgram("vertex.vert", "fragment.frag");

        if (!program) {
			ID = 0;
			RZ_CORE_ERROR("PickBufferShader failed to create shader program will default to shader in slot 0");
			return;
		}
		ID = *program;
    }

}