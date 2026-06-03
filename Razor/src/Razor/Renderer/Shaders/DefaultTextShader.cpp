#include "DefaultTextShader.h"
#include "../../ShaderReader.h"
#include "../../Log.h"

namespace Razor
{
    DefaultTextShader::DefaultTextShader()
    {
        Scope<unsigned int> program = ShaderReader::CreateShaderProgram("textVertex.vert", "textFrag.frag");
        if (!program) {
			ID = 0;
			RZ_CORE_ERROR("PickBufferShader failed to create shader program will default to shader in slot 0");
			return;
		}
		ID = *program;
    }
}