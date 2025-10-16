#include "PickBufferShader.h"
#include "../../ShaderReader.h"

namespace Razor
{
	PickBufferShader::PickBufferShader() : Shader()
	{
		ID = ShaderReader::CreateShaderProgram("pickbuffer.vert", "pickbuffer.frag");
	}
}
