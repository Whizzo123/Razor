#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/gtc/type_ptr.hpp>

namespace Razor
{
    //void SetTextures(std::vector<MeshData::Texture>& Textures)
    //{
    //    unsigned int DiffuseNr = 1;
    //    unsigned int SpecularNr = 1;
    //    unsigned int NormalNr = 1;
    //    unsigned int HeightNr = 1;
    //    int count = 0;
    //    for (const MeshData::Texture& Texture : Textures)
    //    {
    //        std::string number;
    //        std::string name = Texture.Type;
    //        if (name == "texture_diffuse")
    //            number = std::to_string(DiffuseNr++);
    //        else if (name == "texture_specular")
    //            number = std::to_string(SpecularNr++); // transfer unsigned int to string
    //        else if (name == "texture_normal")
    //            number = std::to_string(NormalNr++); // transfer unsigned int to string
    //        else if (name == "texture_height")
    //            number = std::to_string(HeightNr++); // transfer unsigned int to string
    //        //SetInt((name + number), count);
    //        glActiveTexture(GL_TEXTURE0 + count);
    //        glBindTexture(GL_TEXTURE_2D, Texture.Id);
    //        count++;
    //    }
    //}

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