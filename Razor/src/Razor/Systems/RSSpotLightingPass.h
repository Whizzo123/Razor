#pragma once
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{
	struct SpotLight;

    class RSSpotLightingPass : public RenderSystem
    {
	public:
		RSSpotLightingPass(Ref<Scene> Scene) : RenderSystem(Scene)
		{
			SystemRenderStage = RenderStage::RENDER_STAGE_LIGHTING_PASS;
		}
		void Render(RenderPipelineEntityProperties& Properties) override;
    };
}

