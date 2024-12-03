#pragma once
#include "System.h"
#include "../Renderer/IRenderer.h"
#include "../Coordinator.h"
#include "../Renderer/Shaders/Shader.h"

namespace Razor
{
	class RSPickBufferRenderPass : public RenderSystem
	{
	public:
		RSPickBufferRenderPass(std::shared_ptr<IRenderer> EngineRenderer, std::unordered_map<uint8_t, std::shared_ptr<Shader>>& IDToShaderMap)
		{
			Coordinator = Coordinator::GetInstance();
			SystemRenderStage = RenderStage::RENDER_STAGE_RENDER;
			Renderer = EngineRenderer;
			ShaderMap = IDToShaderMap;

			Signature = { Coordinator->GetComponentType<Mesh>(), Coordinator->GetComponentType<Material>() };
		}
		void Render(RenderPipelineEntityProperties& Properties) override;

	protected:
		std::shared_ptr<Coordinator> Coordinator;
		std::shared_ptr<IRenderer> Renderer;
		std::unordered_map<uint8_t, std::shared_ptr<Shader>> ShaderMap;
	};
}

