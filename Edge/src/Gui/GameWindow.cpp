#include "GameWindow.h"


namespace EdgeEditor
{

    GameWindow::GameWindow(Razor::Ref<EditorStorage> storage)
    {
    	_mGamePipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_MATERIAL_PASS, std::vector<const char*> { typeid(Razor::RSMaterialPass).name() } });
		_mGamePipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_TEXT_PASS, std::vector<const char*> { typeid(Razor::RSTextPass).name() } });
		_mGamePipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_LIGHTING_PASS, std::vector<const char*>
		{
			typeid(Razor::RSDirectionalLightingPass).name(),
			typeid(Razor::RSPointLightingPass).name(),
			typeid(Razor::RSSpotLightingPass).name()
		} });
		_mGamePipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_TRANSFORMATION_PASS, std::vector<const char*> { typeid(Razor::RSTransformationsPass).name() } });
		_mGamePipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_CAMERA_PASS, std::vector<const char*> { typeid(Razor::RSCameraPass).name() } });
		_mGamePipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_RENDER, std::vector<const char*> { typeid(Razor::RSRenderPass).name() } });

        _mGameBuffer = Razor::Engine::Get().mRenderer->CreateFrameBuffer(300, 200);

		_mStorage = storage;
    }

    void GameWindow::Render()
    {
        const uint32_t SIZEX     = static_cast<uint32_t>(std::max(0.0f, _mViewportSize.X));
		const uint32_t SIZEY     = static_cast<uint32_t>(std::max(0.0f, _mViewportSize.Y));

        _mGameBuffer->Refresh(SIZEX, SIZEY);
        Razor::Engine::Get().GetRenderer()->SetViewport(0, 0, SIZEX, SIZEY);
		Razor::Engine::Get().SetGameCameraViewportSize(SIZEX, SIZEY);

        Razor::Engine::Get().Render(_mGameBuffer->GetID(), _mGamePipelineConfig);

        bool bIsOpen;
		Razor::RazorImGui::Begin("Game", &bIsOpen, Razor::RazorGuiWindowFlags_NoScrollbar);
		bGameViewHovered = Razor::RazorImGui::IsWindowHovered();
		_mViewportSize = Razor::Vector2(Razor::RazorImGui::GetContentRegionAvail().X, Razor::RazorImGui::GetContentRegionAvail().Y);
		Razor::RazorImGui::Image(reinterpret_cast<void*>(_mGameBuffer->GetTexture()), Razor::Vector2(_mViewportSize.X, _mViewportSize.Y), Razor::Vector2(0, 1), Razor::Vector2(1, 0));
		Razor::RazorImGui::End();
    }

	void GameWindow::ProcessInput()
	{
		const bool bRuntimeRunning = Razor::Engine::Get().IsRuntimeRunning();

		// Game input is only enabled when playing and the Game panel is hovered
		Razor::Engine::Get().SetGameInputEnabled(bRuntimeRunning && bGameViewHovered);	
	}
}