#pragma once

#include "Razor.h"

namespace EdgeEditor
{
	class RSEditorCamera : public Razor::RenderSystem
	{
	public:
		RSEditorCamera(Razor::Ref<Razor::Scene> Scene, std::shared_ptr<Razor::IRenderer> Renderer, Razor::Camera& EditorCamera) 
			: RenderSystem(Scene), Renderer(Renderer), EditorCamera(EditorCamera)
		{
			SystemRenderStage = Razor::RenderStage::RENDER_STAGE_CAMERA_PASS;
		}
		void Render(Razor::RenderPipelineEntityProperties& EntityProperties) override;

	protected:
		std::shared_ptr<Razor::IRenderer> Renderer;
		Razor::Camera& EditorCamera;
	};
}

