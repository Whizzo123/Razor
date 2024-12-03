#pragma once
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{
	struct PointLight;


    class RSPointLightingPass : public RenderSystem
    {
	public:
		RSPointLightingPass()
		{
			Coordinator = Coordinator::GetInstance();
			SystemRenderStage = RenderStage::RENDER_STAGE_LIGHTING_PASS;

			Signature = { Coordinator->GetComponentType<PointLight>() };
		}
		void Render(RenderPipelineEntityProperties& Properties) override;

	protected:
		std::shared_ptr<Coordinator> Coordinator;
    };
}

