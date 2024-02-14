#pragma once
#include "Shader.h"
#include "../../ShaderReader.h"
#include "../../Log.h"
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

		void Setup(CameraInfo* CamInfo, glm::mat4 model, MaterialData Mat, std::shared_ptr<std::vector<Light*>> Light) override;

	};

}

