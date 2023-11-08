#include "DefaultMeshShader.h"

namespace Razor
{

    void DefaultMeshShader::Setup(glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec3 lightColor)
    {
        SetMat4Float("projection", projection);
        SetMat4Float("view", view);
        SetMat4Float("model", model);
    }

}