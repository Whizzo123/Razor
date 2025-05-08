#pragma once
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{
	struct DirectionalLight;


	class RSDirectionalLightingPass : public RenderSystem
	{
	public:
		RSDirectionalLightingPass(Ref<Razor::Scene> Scene) : RenderSystem(Scene)
		{
			SystemRenderStage = RenderStage::RENDER_STAGE_LIGHTING_PASS;
		}
		void Render(RenderPipelineEntityProperties& Properties) override;
	};
}

