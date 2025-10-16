#pragma once
#include "Shader.h"

namespace Razor
{

	class DebugLightShader : public Shader
	{
	public:
		DebugLightShader();
		void Setup(CameraInfo* CameraInfo, glm::mat4 Model, MaterialData Mat, std::shared_ptr<std::vector<Light*>> Lights) override;
	};

}

