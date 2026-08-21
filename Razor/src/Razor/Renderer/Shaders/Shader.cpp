#include "Shader.h"

#include "../../Component.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <string>

namespace Razor
{

    unsigned int Shader::GetUniformLocation(std::string Name)
    {
        return glGetUniformLocation(ID, Name.c_str());
    }

    void Shader::SetInt(std::string Name, int Value)
    {
        unsigned int Loc = GetUniformLocation(Name);
        glUniform1i(Loc, Value);
    }

    void Shader::SetMat4Float(std::string Name, glm::mat4 Value)
    {
        unsigned int Loc = GetUniformLocation(Name);
        glUniformMatrix4fv(Loc, 1, false, glm::value_ptr(Value));
    }

    void Shader::SetVec3(std::string Name, glm::vec3 Value)
    {
        unsigned int Loc = GetUniformLocation(Name);
        glUniform3f(Loc, Value.x, Value.y, Value.z);
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