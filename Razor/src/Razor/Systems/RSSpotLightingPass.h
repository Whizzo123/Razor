#pragma once
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{
    class RSSpotLightingPass : public RenderSystem
    {
	public:
		RSSpotLightingPass()
		{
			Coordinator = Coordinator::GetInstance();
			SystemRenderStage = RenderStage::RENDER_STAGE_LIGHTING_PASS;
		}
		void Render(RenderPipelineEntityProperties& Properties) override;

	protected:
		std::shared_ptr<Coordinator> Coordinator;
    };
}

