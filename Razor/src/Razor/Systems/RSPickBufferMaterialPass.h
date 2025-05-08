#pragma once

#include "System.h"
#include "../Coordinator.h"

namespace Razor
{
	struct Mesh;
	struct Material;

	class RSPickBufferMaterialPass : public RenderSystem
	{
	public:
		RSPickBufferMaterialPass(Ref<Razor::Scene> Scene) : RenderSystem(Scene)
		{
			SystemRenderStage = RenderStage::RENDER_STAGE_MATERIAL_PASS;
		}
		void Render(RenderPipelineEntityProperties& Properties) override;
	};
}

