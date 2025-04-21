#pragma once
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{
	struct Material;

	class RSMaterialPass : public RenderSystem
	{
	public:
		RSMaterialPass(Ref<Razor::Scene> Scene) : RenderSystem(Scene)
		{
			SystemRenderStage = RenderStage::RENDER_STAGE_MATERIAL_PASS;
		}
		void Render(RenderPipelineEntityProperties& Properties) override;
	};
}

