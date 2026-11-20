#pragma once
#include "System.h"
#include "../Coordinator.h"
#include "../Renderer/IRenderer.h"

namespace Razor
{

	class RSCameraPass : public RenderSystem
	{
	public:
		RSCameraPass(Ref<Scene> Scene, std::shared_ptr<IRenderer> Renderer) : RenderSystem(Scene), Renderer(Renderer)
		{
			SystemRenderStage = RenderStage::RENDER_STAGE_CAMERA_PASS;
		}
		void Render(RenderPipelineData& data) override;

		void SetViewportSize(uint32_t w, uint32_t h) { ViewportWidth = w; ViewportHeight = h; }

	protected:
		std::shared_ptr<IRenderer> Renderer;
		uint32_t ViewportWidth = 800;
		uint32_t ViewportHeight = 600;
	};

}

