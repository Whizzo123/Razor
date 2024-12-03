#pragma once
#include "System.h"
#include "../Coordinator.h"
#include "../Renderer/IRenderer.h"

namespace Razor
{

	class RSCameraPass : public RenderSystem
	{
	public:
		RSCameraPass(std::shared_ptr<IRenderer> Renderer)
		{
			SystemRenderStage = RenderStage::RENDER_STAGE_CAMERA_PASS;
			Coordinator = Coordinator::GetInstance();
			this->Renderer = Renderer;

			Signature = { Coordinator->GetComponentType<Camera>() };
		}
		void Render(RenderPipelineEntityProperties& EntityProperties) override;

	protected:
		std::shared_ptr<Coordinator> Coordinator;
		std::shared_ptr<IRenderer> Renderer;
	};

}

