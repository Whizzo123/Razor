#pragma once
#include "Shader.h"
#include "../../ShaderReader.h"

namespace Razor
{

	class DebugLightShader : public Shader
	{
	public:
		DebugLightShader() : Shader()
		{
			ID = ShaderReader::CreateShaderProgram("lightvertex.vert", "lightfragment.frag");
		}
		~DebugLightShader() {}

		void Setup(CameraInfo* CameraInfo, glm::mat4 Model, MaterialData Mat, std::shared_ptr<std::vector<Light>> Lights) override;
	};

}

