#include "PickBufferShader.h"
#include "../../ShaderReader.h"
#include "../../Log.h"

namespace Razor
{
	PickBufferShader::PickBufferShader() : Shader()
	{
		Scope<unsigned int> program = ShaderReader::CreateShaderProgram("pickbuffer.vert", "pickbuffer.frag");
		if (!program) {
			ID = 0;
			RZ_CORE_ERROR("PickBufferShader failed to create shader program will default to shader in slot 0");
			return;
		}
		ID = *program;
	}
}
