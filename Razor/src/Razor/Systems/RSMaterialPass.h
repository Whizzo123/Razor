#pragma once
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{
	class RSMaterialPass : public RenderSystem
	{
	public:
		RSMaterialPass()
		{
			Coordinator = Coordinator::GetInstance();
			SystemRenderStage = RenderStage::RENDER_STAGE_MATERIAL_PASS;
		}
		void Render(std::array<RenderPipelineEntityProperties, MAX_ENTITIES>& Properties) override;

	protected:
		std::shared_ptr<Coordinator> Coordinator;
	};
}

