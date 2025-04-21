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
		void Render(RenderPipelineEntityProperties& EntityProperties) override;

	protected:
		std::shared_ptr<IRenderer> Renderer;
	};

}

