#pragma once
#include "System.h"

namespace Razor
{
    class RSTextPass : public RenderSystem
    {
    public:
		RSTextPass(Ref<Scene> Scene) : RenderSystem(Scene)
		{
			SystemRenderStage = RenderStage::RENDER_STAGE_TEXT_PASS;
		}
		void Render(RenderPipelineData& data) override;
    };
}