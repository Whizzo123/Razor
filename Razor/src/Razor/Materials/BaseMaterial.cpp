#include "BaseMaterial.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../TextureReader.h"

BaseMaterial::BaseMaterial(std::string VertexShader, std::string FragmentShader, std::vector<std::string>& TextureNames) 
	: Material(VertexShader, FragmentShader, TextureNames)
{
	for (const std::string& Name : TextureNames)
	{
		Textures.push_back(TextureReader::CreateTexture(Name));
	}
}

void BaseMaterial::PassValues(Transform ObjTransform)
{
	glUseProgram(ID);
	Shader.SetInt("ourTexture", Textures[0]);
	Shader.SetInt("ourTexture2", Textures[1]);
	glm::mat4 trans = ObjTransform.Get();
	Shader.SetMat4Float("transform", trans);
}
