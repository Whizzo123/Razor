#pragma once
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{
	struct SpotLight;

    class RSSpotLightingPass : public RenderSystem
    {
	public:
		RSSpotLightingPass()
		{
			Coordinator = Coordinator::GetInstance();
			SystemRenderStage = RenderStage::RENDER_STAGE_LIGHTING_PASS;

			Signature = { Coordinator->GetComponentType<SpotLight>() };
		}
		void Render(RenderPipelineEntityProperties& Properties) override;

	protected:
		std::shared_ptr<Coordinator> Coordinator;
    };
}

