#pragma once
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{
	struct DirectionalLight;


	class RSDirectionalLightingPass : public RenderSystem
	{
	public:
		RSDirectionalLightingPass()
		{
			Coordinator = Coordinator::GetInstance();
			SystemRenderStage = RenderStage::RENDER_STAGE_LIGHTING_PASS;

			Signature = { Coordinator->GetComponentType<DirectionalLight>() };
		}
		void Render(RenderPipelineEntityProperties& Properties) override;

	protected:
		std::shared_ptr<Coordinator> Coordinator;
	};
}

