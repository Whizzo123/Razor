#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

void Shader::AddTextures(std::vector<unsigned int>& InTextures)
{
    for (unsigned int Texture : InTextures)
    {
        Textures.push_back(Texture);
    }
}

void Shader::SetTextures()
{
    int count = 0;
    for(unsigned int Texture : Textures)
    {
        SetInt("ourTexture" + std::to_string(count), Texture);
        glActiveTexture(GL_TEXTURE0 + count);
        glBindTexture(GL_TEXTURE_2D, Texture);
        count++;
    }
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

void Shader::SetMat4Float(std::string Name, glm::mat4 Value)
{
    unsigned int Loc = GetUniformLocation(Name);
    glUniformMatrix4fv(Loc, 1, false, glm::value_ptr(Value));
}
