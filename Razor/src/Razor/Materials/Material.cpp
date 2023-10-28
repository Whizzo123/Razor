#include "Material.h"
#include "../ShaderReader.h"

Material::Material(std::string VertexShader, std::string FragmentShader, std::vector<std::string>& TextureNames)
{
	ID = ShaderReader::CreateShaderProgram("vertex.vert", "fragment.frag");
}

Material::~Material()
{
}

void Material::BindMaterial()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Textures[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Textures[1]);
}
