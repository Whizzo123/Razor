#pragma once
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{
	struct Transform;

	class RSTransformationsPass : public RenderSystem
	{
	public:
		RSTransformationsPass(Ref<Razor::Scene> Scene) : RenderSystem(Scene)
		{
			SystemRenderStage = RenderStage::RENDER_STAGE_TRANSFORMATION_PASS;
		}
		void Render(RenderPipelineEntityProperties& EntityProperties) override;
	};
}

