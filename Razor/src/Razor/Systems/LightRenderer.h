#pragma once

#include "../Coordinator.h"
#include "../Renderer/Shaders/Shader.h"
#include "../Renderer/IRenderer.h"

namespace Razor
{

	class LightRenderer : public System
	{
	public:
		LightRenderer(Ref<Razor::Scene> Scene, std::shared_ptr<IRenderer> Renderer, std::shared_ptr<std::vector<Light*>> SceneLights) 
			: System(Scene), Renderer(Renderer), SceneLights(SceneLights) {}
		void Init() override;
		void Run(float dt) override;

	private:
		std::shared_ptr<IRenderer> Renderer;
		std::shared_ptr<std::vector<Light*>> SceneLights;
		Shader LightShader;
	};
}

