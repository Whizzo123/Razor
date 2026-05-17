#include "DefaultTextShader.h"
#include "../../ShaderReader.h"

namespace Razor
{
    DefaultTextShader::DefaultTextShader()
    {
        ID = ShaderReader::CreateShaderProgram("textVertex.vert", "textFrag.frag");
    }
}