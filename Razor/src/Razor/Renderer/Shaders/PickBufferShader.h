#pragma once

#include "Shader.h"
#include "../../ShaderReader.h"

namespace Razor
{
	class PickBufferShader : public Shader
	{
	public:
		PickBufferShader() : Shader()
		{
			ID = ShaderReader::CreateShaderProgram("pickbuffer.vert", "pickbuffer.frag");
		}
		~PickBufferShader() {}

		void Setup(CameraInfo* CameraInfo, glm::mat4 Model, MaterialData Mat, std::shared_ptr<std::vector<Light*>> Lights) override;
	};
}

