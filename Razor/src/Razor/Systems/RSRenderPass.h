#pragma once
#include "System.h"
#include "../Coordinator.h"
#include "../Renderer/IRenderer.h"
#include "../Renderer/Shaders/Shader.h"
namespace Razor
{
	class RSRenderPass : public RenderSystem
	{
		
		public:
			RSRenderPass(Ref<Razor::Scene> Scene, std::shared_ptr<IRenderer> EngineRenderer, std::unordered_map<uint8_t, std::shared_ptr<Shader>>& IDToShaderMap)
				: RenderSystem(Scene)
			{
				SystemRenderStage = RenderStage::RENDER_STAGE_RENDER;
				Renderer = EngineRenderer;
				ShaderMap = IDToShaderMap;
			}
			void Render(RenderPipelineEntityProperties& Properties) override;

		protected:
			std::shared_ptr<IRenderer> Renderer;
			std::unordered_map<uint8_t, std::shared_ptr<Shader>> ShaderMap;
	};
}

