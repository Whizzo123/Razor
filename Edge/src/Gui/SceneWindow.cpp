#include "SceneWindow.h"


namespace EdgeEditor
{

    SceneWindow::SceneWindow(Razor::Ref<EditorStorage> storage)
    {
        _mEditorPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_MATERIAL_PASS, std::vector<const char*> { typeid(Razor::RSMaterialPass).name() } });
	    _mEditorPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_TEXT_PASS, std::vector<const char*> { typeid(Razor::RSTextPass).name() } });
	    _mEditorPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_LIGHTING_PASS, std::vector<const char*>
	    {
		    typeid(Razor::RSDirectionalLightingPass).name(),
		    typeid(Razor::RSPointLightingPass).name(),
		    typeid(Razor::RSSpotLightingPass).name()
	    } });
	    _mEditorPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_TRANSFORMATION_PASS, std::vector<const char*> { typeid(Razor::RSTransformationsPass).name() } });
	    _mEditorPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_CAMERA_PASS, std::vector<const char*> { typeid(EdgeEditor::RSEditorCamera).name() } });
	    _mEditorPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_RENDER, std::vector<const char*> { typeid(Razor::RSRenderPass).name() } });

	
    	_mPickPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_MATERIAL_PASS, std::vector<const char*> { typeid(Razor::RSPickBufferMaterialPass).name() } });
    	_mPickPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_TRANSFORMATION_PASS, std::vector<const char*> { typeid(Razor::RSTransformationsPass).name() } });
    	_mPickPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_CAMERA_PASS, std::vector<const char*> { typeid(EdgeEditor::RSEditorCamera).name() } });
    	_mPickPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_RENDER, std::vector<const char*> { typeid(Razor::RSPickBufferRenderPass).name() } });

        _mSceneBuffer = Razor::Engine::Get().mRenderer->CreateFrameBuffer(300, 200);
		_mPickBuffer = Razor::Engine::Get().mRenderer->CreateFrameBuffer(300, 200);

		_mEditorCameraSystem = Razor::Engine::Get().GetCoordinator()->RegisterSystem<EdgeEditor::RSEditorCamera>(EdgeEditor::RSEditorCamera(Razor::Engine::Get().mCurrentScene, Razor::Engine::Get().GetRenderer(), _mEditorCamera.GetCamera()));

		_mStorage = storage;
    }

    void SceneWindow::Render()
    {
        const uint32_t SizeX     = static_cast<uint32_t>(std::max(0.0f, _mViewportSize.X));
		const uint32_t SizeY     = static_cast<uint32_t>(std::max(0.0f, _mViewportSize.Y));

        _mSceneBuffer->Refresh(SizeX, SizeY);
		_mPickBuffer->Refresh(SizeX, SizeY);
        Razor::Engine::Get().GetRenderer()->SetViewport(0, 0, SizeX, SizeY);
		_mEditorCameraSystem->SetViewportSize(SizeX, SizeY);
        // TODO you need another call like this for the game view with it's size
        Razor::Engine::Get().Render(_mSceneBuffer->GetID(), _mEditorPipelineConfig);

		Razor::Engine::Get().PopulateRenderPipelineDebugData();

		Razor::Engine::Get().Render(_mPickBuffer->GetID(), _mPickPipelineConfig);

		Razor::Engine::Get().ClearDebugDrawBuffer();

        bool bIsOpen;
	    Razor::RazorImGui::Begin("Scene", &bIsOpen, Razor::RazorGuiWindowFlags_MenuBar | Razor::RazorGuiWindowFlags_NoScrollbar);
	    bSceneViewHovered = Razor::RazorImGui::IsWindowHovered();
	    _mViewportSize = Razor::Vector2(Razor::RazorImGui::GetContentRegionAvail().X, Razor::RazorImGui::GetContentRegionAvail().Y);
	    Razor::RazorImGui::Image(reinterpret_cast<void*>(_mSceneBuffer->GetTexture()), Razor::Vector2(_mViewportSize.X, _mViewportSize.Y), Razor::Vector2(0, 1), Razor::Vector2(1, 0));
	    _mViewportPos = Razor::RazorImGui::GetItemRectMin();
	    Razor::RazorImGui::End();
    }

	void SceneWindow::ProcessInput()
	{
		const bool bRuntimeRunning = Razor::Engine::Get().IsRuntimeRunning();
		// Editor camera gets input when not in play mode, or when hovering the Scene panel
		if (!bRuntimeRunning || bSceneViewHovered)
		{
			_mEditorCamera.ProcessInput(0.01f);
		}

		// Object picking only fires from within the Scene panel
		if (bSceneViewHovered && Razor::RazorIO::Get().GetStateForMouseButton(Razor::LEFT) == Razor::MOUSE_DOWN)
		{
			Razor::Vector2D mousePos = Razor::RazorIO::Get().CurrentMousePos;
			unsigned int offsetMousePosX = mousePos.X - _mViewportPos.X;
			unsigned int offsetMousePosY = mousePos.Y - _mViewportPos.Y;

			if (offsetMousePosX < 0 || offsetMousePosX > _mPickBuffer->GetWidth() || offsetMousePosY < 0 || offsetMousePosY > _mPickBuffer->GetHeight())
			{
				return;
			}
			PickObject(Razor::Vector2(offsetMousePosX, offsetMousePosY));
		}
	}

	void SceneWindow::PickObject(Razor::Vector2 mousePos)
	{
		float pixel[3];
		Razor::Engine::Get().GetRenderer()->ReadPixels(mousePos.X, _mPickBuffer->GetHeight() - mousePos.Y, 1, 1, pixel, _mPickBuffer->GetID());
		std::uint32_t pickedEntity = 0;
		pickedEntity = static_cast<std::uint32_t>(pixel[0] * 255.0f) + (std::uint32_t(pixel[1] * 255.0f) << 8)
				+ (std::uint32_t(pixel[2] * 255.0f) << 16);
		_mStorage->SelectedEntity = Razor::Engine::Get().mCurrentScene->GetEntity(entt::entity(pickedEntity));
	}
}