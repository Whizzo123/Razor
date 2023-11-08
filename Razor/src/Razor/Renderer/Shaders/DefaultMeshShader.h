#pragma once
#include "Shader.h"
#include "../../ShaderReader.h"
namespace Razor
{

	class DefaultMeshShader : public Shader
	{
	public:
		DefaultMeshShader() : Shader()
		{
			ID = ShaderReader::CreateShaderProgram("vertex.vert", "fragment.frag");
		}
		~DefaultMeshShader(){}

		void Setup(glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec3 lightColor) override;

	};

}

