#pragma once
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{
	class RSTransformationsPass : public RenderSystem
	{
	public:
		RSTransformationsPass()
		{
			SystemRenderStage = RenderStage::RENDER_STAGE_TRANSFORMATION_PASS;
			Coordinator = Coordinator::GetInstance();
		}
		void Render(RenderPipelineEntityProperties& EntityProperties) override;

	protected:
		std::shared_ptr<Coordinator> Coordinator;
	};
}

