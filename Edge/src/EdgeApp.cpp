#include <Razor.h>
#include <Razor/EntryPoint.h>
#include <algorithm>

#include "Inspector.h"
#include "SceneView.h"
#include "EditorStorage.h"
#include "ProjectExplorer.h"
#include "Systems/RSEditorCamera.h"
#include "EditorCamera.h"
#include "Gui/NewProjectPopupWindow.h"
#include "Gui/OpenProjectPopupWindow.h"
#include "SystemView.h"
#include <Razor/Systems/RSCameraPass.h>



class Edge : public Razor::Application
{
public:
	Edge() {};
	~Edge() {};
	void Run() override;
	/**
	* Processes input from the user for the editor as well as calls Engine::ProcessInput()
	*/
	void ProcessInput();
	void RenderSceneViewport(Razor::Ref<Razor::Framebuffer> SceneBuffer);
	void RenderGameViewport(Razor::Ref<Razor::Framebuffer> GameBuffer);
	/**
	* Function to pick and object via the PickBuffer
	* Needs it's functionality properly implemented
	*
	* @param PickBuffer - Shared Ptr to the FrameBuffer used as the PickBuffer
	* @param Offset - 2D vector to offset mouse position based on viewport position
	*/
	void PickObject(Razor::Vector2 Offset);
	/**
	* Function to create an ImGui dockspace
	* 
	* @param Title - string to use as title
	*/
	void CreateDockspace(const std::string& Title);
	/**
	* Function to load new project into editor
	*/
	void OnNewProjectSet();

private:
	EdgeEditor::PopupWindow* CurrentPopup = nullptr; /** Holds current popup open */

private:
	EdgeEditor::EditorCamera EditorCamera; /** Object to house and control the camera we will use for rendering the scene to the viewport */
	Razor::Ref<Razor::Framebuffer> PickBuffer; /** Framebuffer object for picking objects */
	Razor::Ref<Razor::Framebuffer> SceneBuffer; /** Framebuffer object for rendering scene */
	Razor::Ref<Razor::Framebuffer> GameBuffer; /** Framebuffer object for rendering game camera view */
	bool bSceneViewHovered = false; /** True when cursor hovers the Scene viewport panel */
	bool bGameViewHovered  = false; /** True when cursor hovers the Game viewport panel */
	Razor::Vector2 ViewportSize { 0.0f, 0.0f }; /** 2D vector to hold size of viewport window */
	Razor::Vector2 GameViewportSize { 0.0f, 0.0f }; /** 2D vector to hold size of Game viewport panel */
	Razor::Vector2 ViewportPos { 0.0f, 0.0f }; /** 2D vector to hold position of image displaying scene texture for viewport*/
	Razor::Ref<EdgeEditor::EditorStorage> Storage; /** Container object to hold data to be shared among windows*/
	EdgeEditor::ProjectExplorer _mProjectExplorerWindow;
	Razor::Ref<Razor::Scene> _mPlaybackSceneBackup;
	Razor::Ref<EdgeEditor::RSEditorCamera> _mEditorCameraSystem;
};

Razor::Application* Razor::CreateApplication()
{
	return new Edge();
}

void Edge::Run()
{
	Razor::Engine& Engine = Razor::Engine::Get();

	_mEditorCameraSystem = Engine.GetCoordinator()->RegisterSystem<EdgeEditor::RSEditorCamera>(EdgeEditor::RSEditorCamera(Engine.mCurrentScene, Engine.GetRenderer(), EditorCamera.GetCamera()));

	// RSCameraPass is registered by Engine::InitSystems() — no explicit RegisterSystem call needed here
	Razor::RenderPipelineConfig GamePipelineConfig;
	GamePipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_MATERIAL_PASS, std::vector<const char*> { typeid(Razor::RSMaterialPass).name() } });
	GamePipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_TEXT_PASS, std::vector<const char*> { typeid(Razor::RSTextPass).name() } });
	GamePipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_LIGHTING_PASS, std::vector<const char*>
	{
		typeid(Razor::RSDirectionalLightingPass).name(),
		typeid(Razor::RSPointLightingPass).name(),
		typeid(Razor::RSSpotLightingPass).name()
	} });
	GamePipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_TRANSFORMATION_PASS, std::vector<const char*> { typeid(Razor::RSTransformationsPass).name() } });
	GamePipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_CAMERA_PASS, std::vector<const char*> { typeid(Razor::RSCameraPass).name() } });
	GamePipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_RENDER, std::vector<const char*> { typeid(Razor::RSRenderPass).name() } });

	Razor::RenderPipelineConfig EditorPipelineConfig;
	EditorPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_MATERIAL_PASS, std::vector<const char*> { typeid(Razor::RSMaterialPass).name() } });
	EditorPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_TEXT_PASS, std::vector<const char*> { typeid(Razor::RSTextPass).name() } });
	EditorPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_LIGHTING_PASS, std::vector<const char*>
	{
		typeid(Razor::RSDirectionalLightingPass).name(),
		typeid(Razor::RSPointLightingPass).name(),
		typeid(Razor::RSSpotLightingPass).name()
	} });
	EditorPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_TRANSFORMATION_PASS, std::vector<const char*> { typeid(Razor::RSTransformationsPass).name() } });
	EditorPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_CAMERA_PASS, std::vector<const char*> { typeid(EdgeEditor::RSEditorCamera).name() } });
	EditorPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_RENDER, std::vector<const char*> { typeid(Razor::RSRenderPass).name() } });

	Razor::RenderPipelineConfig PickPipelineConfig;
	PickPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_MATERIAL_PASS, std::vector<const char*> { typeid(Razor::RSPickBufferMaterialPass).name() } });
	PickPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_TRANSFORMATION_PASS, std::vector<const char*> { typeid(Razor::RSTransformationsPass).name() } });
	PickPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_CAMERA_PASS, std::vector<const char*> { typeid(EdgeEditor::RSEditorCamera).name() } });
	PickPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_RENDER, std::vector<const char*> { typeid(Razor::RSPickBufferRenderPass).name() } });

	

	Engine.InitSystems();

	SceneBuffer = Engine.mRenderer->CreateFrameBuffer(300, 200);
	PickBuffer  = Engine.mRenderer->CreateFrameBuffer(300, 200);
	GameBuffer  = Engine.mRenderer->CreateFrameBuffer(300, 200);

	// Remove this should be better way of grabbing default models
	const char* path = "resources/models/Cube.obj";
	Razor::Model defaultModel;
	// TODO: The problem here is actually that there is no AssetDirectory until a project has been loaded so maybe we need to think about how we handle the untitled project?
	if (fopen(path, "r")) {
		defaultModel.LoadMesh(path);
		Engine.GetRenderer()->SetupMesh(defaultModel.GetModelMeshData());
	}
	

	if (std::shared_ptr<Razor::Shader> DefaultShader = Engine.GetShaderForType(typeid(Razor::DefaultMeshShader).name()))
	{
		defaultModel.SetModelShader(DefaultShader->ID);
	}

	Storage = std::make_shared<EdgeEditor::EditorStorage>();
	Storage->DefaultModel = defaultModel;
	Storage->OnProjectSet().AddRaw(this, &Edge::OnNewProjectSet);

	EdgeEditor::Inspector InspectorWindow(Storage);
	EdgeEditor::SceneView SceneViewWindow(Storage);
	_mProjectExplorerWindow = EdgeEditor::ProjectExplorer(Storage);
	EdgeEditor::SystemView SystemViewWindow;

	Razor::SceneSerializer::Deserialize(Engine.mCurrentScene);
	while (!Engine.ShouldEngineClose())
	{
		std::shared_ptr<Razor::IRenderer> Renderer = Engine.mRenderer;

		const uint32_t SizeX     = static_cast<uint32_t>(std::max(0.0f, ViewportSize.X));
		const uint32_t SizeY     = static_cast<uint32_t>(std::max(0.0f, ViewportSize.Y));
		const uint32_t GameSizeX = static_cast<uint32_t>(std::max(0.0f, GameViewportSize.X));
		const uint32_t GameSizeY = static_cast<uint32_t>(std::max(0.0f, GameViewportSize.Y));

		PickBuffer->Refresh(SizeX, SizeY);
		SceneBuffer->Refresh(SizeX, SizeY);
		GameBuffer->Refresh(GameSizeX, GameSizeY);
		Renderer->SetViewport(0, 0, SizeX, SizeY);

		_mEditorCameraSystem->SetViewportSize(SizeX, SizeY);
		Engine.SetGameCameraViewportSize(GameSizeX, GameSizeY);

		Engine.PopulateRenderPipelineDebugData();

		Engine.Render(PickBuffer->GetID(), PickPipelineConfig);

		ProcessInput();

		Engine.Render(SceneBuffer->GetID(), EditorPipelineConfig);
		Engine.Render(GameBuffer->GetID(), GamePipelineConfig);

		Engine.ClearDebugDrawBuffer();

		Renderer->PollForEvents();

		Engine.GetGUI().BeginNewFrame();
		CreateDockspace("Edge");
		InspectorWindow.Render();
		SceneViewWindow.Render();
		_mProjectExplorerWindow.Render();
		SystemViewWindow.Render();
		RenderSceneViewport(SceneBuffer);
		RenderGameViewport(GameBuffer);
		Razor::RazorImGui::ShowMetricsWindow();
		Razor::RazorImGui::End();
		if (CurrentPopup)
		{
			if (CurrentPopup->Draw() == false)
			{
				delete(CurrentPopup);
				CurrentPopup = nullptr;
			}
		}
		Engine.GetGUI().EndFrame(Razor::Engine::Get().GetWindow(), Razor::Engine::Get().mRenderer);
		Engine.mRenderer->SwapBuffer(Razor::Engine::Get().GetWindow());
	}
	Razor::Engine::Get().mRenderer->TerminateRendererAPI();
}

void Edge::RenderSceneViewport(Razor::Ref<Razor::Framebuffer> SceneBuffer)
{
	bool bIsOpen;
	Razor::RazorImGui::Begin("Scene", &bIsOpen, Razor::RazorGuiWindowFlags_MenuBar | Razor::RazorGuiWindowFlags_NoScrollbar);
	bSceneViewHovered = Razor::RazorImGui::IsWindowHovered();
	ViewportSize = Razor::Vector2(Razor::RazorImGui::GetContentRegionAvail().X, Razor::RazorImGui::GetContentRegionAvail().Y);
	Razor::RazorImGui::Image(reinterpret_cast<void*>(SceneBuffer->GetTexture()), Razor::Vector2(ViewportSize.X, ViewportSize.Y), Razor::Vector2(0, 1), Razor::Vector2(1, 0));
	ViewportPos = Razor::RazorImGui::GetItemRectMin();
	Razor::RazorImGui::End();
}

void Edge::RenderGameViewport(Razor::Ref<Razor::Framebuffer> GameBuffer)
{
	bool bIsOpen;
	Razor::RazorImGui::Begin("Game", &bIsOpen, Razor::RazorGuiWindowFlags_NoScrollbar);
	bGameViewHovered = Razor::RazorImGui::IsWindowHovered();
	GameViewportSize = Razor::Vector2(Razor::RazorImGui::GetContentRegionAvail().X, Razor::RazorImGui::GetContentRegionAvail().Y);
	Razor::RazorImGui::Image(reinterpret_cast<void*>(GameBuffer->GetTexture()), Razor::Vector2(GameViewportSize.X, GameViewportSize.Y), Razor::Vector2(0, 1), Razor::Vector2(1, 0));
	Razor::RazorImGui::End();
}

void Edge::ProcessInput()
{
	Razor::Engine::Get().ProcessInput();

	const bool bRuntimeRunning = Razor::Engine::Get().IsRuntimeRunning();

	// Editor camera gets input when not in play mode, or when hovering the Scene panel
	if (!bRuntimeRunning || bSceneViewHovered)
		EditorCamera.ProcessInput(0.01f);

	// Game input is only enabled when playing and the Game panel is hovered
	Razor::Engine::Get().SetGameInputEnabled(bRuntimeRunning && bGameViewHovered);

	// Object picking only fires from within the Scene panel
	if (bSceneViewHovered && Razor::RazorIO::Get().GetStateForMouseButton(Razor::LEFT) == Razor::MOUSE_DOWN)
	{
		Razor::Vector2D MousePos = Razor::RazorIO::Get().CurrentMousePos;
		unsigned int OffsetMousePosX = MousePos.X - ViewportPos.X;
		unsigned int OffsetMousePosY = MousePos.Y - ViewportPos.Y;

		if (OffsetMousePosX < 0 || OffsetMousePosX > PickBuffer->GetWidth() || OffsetMousePosY < 0 || OffsetMousePosY > PickBuffer->GetHeight())
		{
			return;
		}
		PickObject(Razor::Vector2(OffsetMousePosX, OffsetMousePosY));
	}
}

void Edge::PickObject(Razor::Vector2 MousePos)
{
	float Pixel[3];
	Razor::Engine::Get().GetRenderer()->ReadPixels(MousePos.X, PickBuffer->GetHeight() - MousePos.Y, 1, 1, Pixel, PickBuffer->GetID());
	std::uint32_t PickedEntity = 0;
	PickedEntity = static_cast<std::uint32_t>(Pixel[0] * 255.0f) + (std::uint32_t(Pixel[1] * 255.0f) << 8)
			+ (std::uint32_t(Pixel[2] * 255.0f) << 16);
	Storage->SelectedEntity = Razor::Engine::Get().mCurrentScene->GetEntity(entt::entity(PickedEntity));
}

void Edge::CreateDockspace(const std::string& Title)
{
	bool bIsOpen;
	bool bIsDockspaceOpen;
	Razor::RazorGuiWindowFlags_ window_flags = Razor::RazorGuiWindowFlags_MenuBar | Razor::RazorGuiWindowFlags_NoDocking;
	unsigned int viewportId = Razor::RazorImGui::GetMainViewport();
	Razor::RazorImGui::SetNextWindowPos(Razor::RazorImGui::GetViewportPos(viewportId));
	Razor::RazorImGui::SetNextWindowSize(Razor::RazorImGui::GetViewportSize(viewportId));
	Razor::RazorImGui::SetNextWindowViewport(viewportId);
	window_flags |= Razor::RazorGuiWindowFlags_NoTitleBar | Razor::RazorGuiWindowFlags_NoCollapse | Razor::RazorGuiWindowFlags_NoResize | Razor::RazorGuiWindowFlags_NoMove;
	window_flags |= Razor::RazorGuiWindowFlags_NoBringToFrontOnFocus | Razor::RazorGuiWindowFlags_NoNavFocus;
	Razor::RazorImGui::Begin(Title.c_str(), &bIsDockspaceOpen, window_flags);
	if (Razor::RazorImGui::BeginMainMenuBar())
	{
		if (Razor::RazorImGui::BeginMenu("File"))
		{
			if (Razor::RazorImGui::MenuItem("Create Project"))
			{
				CurrentPopup = new EdgeEditor::NewProjectPopupWindow();
			}
			if (Razor::RazorImGui::MenuItem("Open Project"))
			{
				CurrentPopup = new EdgeEditor::OpenProjectPopupWindow(Storage);
			}
			if (Razor::RazorImGui::MenuItem("Save Project"))
			{
				Razor::Engine::Get().SaveProject();
			}
			Razor::RazorImGui::EndMenu();
		}
		if (Razor::RazorImGui::BeginMenu("Run"))
		{
			if (Razor::RazorImGui::MenuItem("Play"))
			{
				if (!Razor::Engine::Get().IsRuntimeRunning())
				{
					Razor::Engine& engine = Razor::Engine::Get();
					_mPlaybackSceneBackup = engine.mCurrentScene->Clone();
					engine.RuntimeStart();
				}
			}
			if (Razor::RazorImGui::MenuItem("Stop"))
			{
				Razor::Engine& engine = Razor::Engine::Get();
				engine.RuntimeStop();
				if (_mPlaybackSceneBackup)
					*engine.mCurrentScene = std::move(*_mPlaybackSceneBackup);
				_mPlaybackSceneBackup = nullptr;
			}
			Razor::RazorImGui::EndMenu();
		}
		Razor::RazorImGui::EndMainMenuBar();
	}
	Razor::RazorImGui::DockSpace(Razor::RazorImGui::GetID(Title.c_str()), Razor::Vector2(0.0f, 0.0f), 0);
}

void Edge::OnNewProjectSet()
{
	Razor::Engine& Engine = Razor::Engine::Get();
	if (Engine.LoadProject(Storage->GetProjectPath()))
	{
		_mProjectExplorerWindow.Refresh(Engine.GetAssetDirectory()->GetRootFolder());
	}
}