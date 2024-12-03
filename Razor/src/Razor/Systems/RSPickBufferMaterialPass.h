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
		RSPickBufferMaterialPass()
		{
			Coordinator = Coordinator::GetInstance();
			SystemRenderStage = RenderStage::RENDER_STAGE_MATERIAL_PASS;

			Signature = { Coordinator->GetComponentType<Mesh>(), Coordinator->GetComponentType<Material>() };
		}
		void Render(RenderPipelineEntityProperties& Properties) override;

	protected:
		std::shared_ptr<Coordinator> Coordinator;
	};
}

