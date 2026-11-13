#include "Shader.h"

#include "../../Component.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../Utils/Vector3.h"
#include "../../Utils/Matrix4.h"
#include <string>

namespace Razor
{

    void Shader::Setup(CameraInfo* CamInfo, Matrix4 model, [[maybe_unused]]MaterialData Mat, [[maybe_unused]]std::shared_ptr<std::vector<Light*>> Light)
    {
        SetMat4Float("projection", CamInfo->CameraProjection);
        SetMat4Float("view", CamInfo->CameraView);
        SetMat4Float("model", model);
    }

    unsigned int Shader::GetUniformLocation(std::string Name)
    {
        return glGetUniformLocation(ID, Name.c_str());
    }

    void Shader::SetInt(std::string Name, int Value)
    {
        unsigned int Loc = GetUniformLocation(Name);
        glUniform1i(Loc, Value);
    }

    void Shader::SetMat4Float(std::string Name, Matrix4 Value)
    {
        unsigned int Loc = GetUniformLocation(Name);
        glUniformMatrix4fv(Loc, 1, false, Value.GetValuePtr());
    }

    void Shader::SetVec3(std::string Name, Vector3 Value)
    {
        unsigned int Loc = GetUniformLocation(Name);
        glUniform3f(Loc, Value.X, Value.Y, Value.Z);
    }

    void Shader::SetFloat(std::string Name, float Value)
    {
        unsigned int Loc = GetUniformLocation(Name);
        glUniform1f(Loc, Value);
    }

    void Shader::SetBool(std::string Name, bool Value)
    {
        unsigned int Loc = GetUniformLocation(Name);
        glUniform1i(Loc, Value);
    }

}