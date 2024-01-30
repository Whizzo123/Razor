#pragma once
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{

	class RSDirectionalLightingPass : public RenderSystem
	{
	public:
		RSDirectionalLightingPass()
		{
			Coordinator = Coordinator::GetInstance();
			SystemRenderStage = RenderStage::RENDER_STAGE_LIGHTING_PASS;
		}
		void Render(RenderPipelineEntityProperties& Properties) override;

	protected:
		std::shared_ptr<Coordinator> Coordinator;
	};
}

