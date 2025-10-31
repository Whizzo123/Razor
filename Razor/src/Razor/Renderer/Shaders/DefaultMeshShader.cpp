#include "DefaultMeshShader.h"
#include "../../ShaderReader.h"

namespace Razor
{

    DefaultMeshShader::DefaultMeshShader() : Shader()
    {
        ID = ShaderReader::CreateShaderProgram("vertex.vert", "fragment.frag");
    }

}