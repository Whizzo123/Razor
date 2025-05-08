#pragma once
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{
	struct PointLight;


    class RSPointLightingPass : public RenderSystem
    {
	public:
		RSPointLightingPass(Ref<Razor::Scene> Scene) : RenderSystem(Scene)
		{
			SystemRenderStage = RenderStage::RENDER_STAGE_LIGHTING_PASS;
		}
		void Render(RenderPipelineEntityProperties& Properties) override;
    };
}

