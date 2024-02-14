#pragma once

#include "Systems/System.h"
#include "Coordinator.h"
#include "Component.h"
#include "ShaderReader.h"
#include "Renderer/Shaders/Shader.h"
#include "Engine.h"

namespace Razor
{
	class RAZOR_API MeshRenderer : public System
	{
	public:
		MeshRenderer(std::shared_ptr<IRenderer> Renderer, std::unordered_map<uint8_t, std::shared_ptr<Shader>>& IDToShaderMap, std::shared_ptr<std::vector<Light*>> SceneLights)
		{
			Coordinator = Razor::Coordinator::GetInstance();
			this->Renderer = Renderer;
			ShaderMap = IDToShaderMap;
			Lights = SceneLights;
		}
		void Run(float dt) override;
		void Init() override;

	private:
		void InitMesh(MeshData& MeshToInit);

	private:
		std::shared_ptr<Razor::Coordinator> Coordinator;
		std::shared_ptr<IRenderer> Renderer;
		std::unordered_map<uint8_t, std::shared_ptr<Shader>> ShaderMap;
		std::shared_ptr<std::vector<Light*>> Lights;
	};
}
