#pragma once

#include "System.h"
#include "../Coordinator.h"

namespace Razor
{

	class RSPickBufferMaterialPass : public RenderSystem
	{
	public:
		RSPickBufferMaterialPass()
		{
			Coordinator = Coordinator::GetInstance();
			SystemRenderStage = RenderStage::RENDER_STAGE_MATERIAL_PASS;
		}
		void Render(RenderPipelineEntityProperties& Properties) override;

	protected:
		std::shared_ptr<Coordinator> Coordinator;
	};
}

