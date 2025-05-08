#pragma once

#include "Systems/System.h"
#include "Coordinator.h"
#include "Component.h"
#include "ShaderReader.h"
#include "Renderer/Shaders/Shader.h"
#include "Engine.h"

namespace Razor
{
	class MeshRenderer : public System
	{
	public:
		MeshRenderer(Ref<Scene> Scene, std::shared_ptr<IRenderer> Renderer, std::unordered_map<uint8_t, std::shared_ptr<Shader>>& IDToShaderMap, std::shared_ptr<std::vector<Light*>> SceneLights)
			: System(Scene)
		{
			this->Renderer = Renderer;
			ShaderMap = IDToShaderMap;
			Lights = SceneLights;
		}
		void Run(float dt) override;
		void Init() override;
		//Remove this back to private once we have some kind of import feature
		static void InitMesh(std::vector<MeshData>& Meshes);
	private:
		

	private:
		std::shared_ptr<IRenderer> Renderer;
		std::unordered_map<uint8_t, std::shared_ptr<Shader>> ShaderMap;
		std::shared_ptr<std::vector<Light*>> Lights;
	};
}
