#pragma once

#include "Razor.h"

namespace EdgeEditor
{
	/**
	* Class is a render system that is responsible for handling the editor camera for the camera pass stage of rendering
	*/
	class RSEditorCamera : public Razor::RenderSystem
	{
	public:
		/**
		* Constructor for RSEditorCamera
		* 
		* @param Scene - shared_ptr to Scene we are working with
		* @param Renderer - shared_ptr to IRenderer object currently being used for rendering
		* @param Camera - ref to Camera used in rendering
		*/
		RSEditorCamera(Razor::Ref<Razor::Scene> Scene, std::shared_ptr<Razor::IRenderer> Renderer, Razor::Camera& EditorCamera) 
			: RenderSystem(Scene), Renderer(Renderer), EditorCamera(EditorCamera)
		{
			SystemRenderStage = Razor::RenderStage::RENDER_STAGE_CAMERA_PASS;
		}
		/**
		* Function to pass in properties to shader needed in rendering to do with the Camera
		* 
		* @param EntityProperties - Shader properties for all the entities in the scene
		*/
		void Render(Razor::RenderPipelineEntityProperties& EntityProperties) override;

	protected:
		std::shared_ptr<Razor::IRenderer> Renderer; /** Shared Ptr to current IRenderer object */
		Razor::Camera& EditorCamera; /** Ref to Camera object */
	};
}

