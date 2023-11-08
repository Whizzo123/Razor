#pragma once
#include "Shader.h"
#include "../../ShaderReader.h"

namespace Razor
{

	class DebugLightShader : public Shader
	{
		DebugLightShader() : Shader()
		{
			ID = ShaderReader::CreateShaderProgram("lightvertex.vert", "lightfragment.frag");
		}
		~DebugLightShader() {}

		void Setup(glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec3 lightColor) override;
	};

}

