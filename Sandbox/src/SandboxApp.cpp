#include <Razor.h>
#include <Razor/EntryPoint.h>




class Sandbox : public Razor::Application
{
public:
	Sandbox() {};
	~Sandbox() {};
	void Run() override;
};

Razor::Application* Razor::CreateApplication()
{
	return new Sandbox();
}



void Sandbox::Run()
{
	Razor::Engine& Engine = Razor::Engine::Get();

	Engine.InitSystems();

	Razor::RenderPipelineConfig _mGamePipelineConfig;
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

	Razor::FilePath projPath("/home/joescobbie/Projects/Razor/Sandbox/Sandbox");
	Engine.LoadProject(projPath);

	while (!Engine.ShouldEngineClose())
	{
		Engine.ProcessInput();

		Engine.GetRenderer()->SetViewport(0, 0, 800, 600);
		Engine.SetGameCameraViewportSize(800, 600);
		Engine.Render(0, _mGamePipelineConfig);

		Engine.mRenderer->PollForEvents();

		Engine.mRenderer->SwapBuffer(Engine.GetWindow());
	}
	Engine.mRenderer->TerminateRendererAPI();
	
}


