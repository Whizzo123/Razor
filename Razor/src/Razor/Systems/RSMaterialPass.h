#pragma once
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{
	struct Material;

	class RSMaterialPass : public RenderSystem
	{
	public:
		RSMaterialPass()
		{
			Coordinator = Coordinator::GetInstance();
			SystemRenderStage = RenderStage::RENDER_STAGE_MATERIAL_PASS;

			Signature = { Coordinator->GetComponentType<Material>() };
		}
		void Render(RenderPipelineEntityProperties& Properties) override;

	protected:
		std::shared_ptr<Coordinator> Coordinator;
	};
}

