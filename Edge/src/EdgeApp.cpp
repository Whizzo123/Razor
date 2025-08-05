#include <Razor.h>
#include <Razor/EntryPoint.h>

#include "Inspector.h"
#include "SceneView.h"
#include "EditorStorage.h"
#include "ProjectExplorer.h"
#include "Systems/RSEditorCamera.h"
#include "EditorCamera.h"
#include "FileIO/ProjectSerializer.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Gui/NewProjectPopupWindow.h"
#include "Gui/OpenProjectPopupWindow.h"



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
	/**
	* Function to pick and object via the PickBuffer
	* Needs it's functionality properly implemented
	*
	* @param PickBuffer - Shared Ptr to the FrameBuffer used as the PickBuffer
	* @param Offset - 2D vector to offset mouse position based on viewport position
	*/
	void PickObject(ImVec2 Offset);
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
	ImVec2 ViewportSize; /** 2D vector to hold size of viewport window */
	ImVec2 ViewportPos; /** 2D vector to hold position of image displaying scene texture for viewport*/
	Razor::Ref<EdgeEditor::EditorStorage> Storage; /** Container object to hold data to be shared among windows*/
};

Razor::Application* Razor::CreateApplication()
{
	return new Edge();
}

void Edge::Run()
{
	Razor::Engine& Engine = Razor::Engine::Get();

	Razor::RenderPipelineConfig PipelineConfig;
	PipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_MATERIAL_PASS, std::vector<const char*> { typeid(Razor::RSMaterialPass).name() } });
	PipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_LIGHTING_PASS, std::vector<const char*>
	{
		typeid(Razor::RSDirectionalLightingPass).name(),
		typeid(Razor::RSPointLightingPass).name(),
		typeid(Razor::RSSpotLightingPass).name()
	} });
	PipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_TRANSFORMATION_PASS, std::vector<const char*> { typeid(Razor::RSTransformationsPass).name() } });
	PipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_CAMERA_PASS, std::vector<const char*> { typeid(Razor::RSCameraPass).name() } });
	PipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_RENDER, std::vector<const char*> { typeid(Razor::RSRenderPass).name() } });

	Razor::RenderPipelineConfig EditorPipelineConfig;
	EditorPipelineConfig.push_back(Razor::RenderStageConfig{ Razor::RenderStage::RENDER_STAGE_MATERIAL_PASS, std::vector<const char*> { typeid(Razor::RSMaterialPass).name() } });
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

	Engine.GetCoordinator()->RegisterSystem<EdgeEditor::RSEditorCamera>(EdgeEditor::RSEditorCamera(Engine.CurrentScene, Engine.GetRenderer(), EditorCamera.GetCamera()));

	Engine.InitSystems();

	SceneBuffer = Engine.Renderer->CreateFrameBuffer(300, 200);
	PickBuffer = Engine.Renderer->CreateFrameBuffer(300, 200);

	Razor::Model DefaultModel = Engine.ProcessModel("resources/models/Cube.obj");
	DefaultModel.SetModelShader(Engine.GetShaderForType(typeid(Razor::DefaultMeshShader).name())->ID);

	Storage = std::make_shared<EdgeEditor::EditorStorage>();
	Storage->DefaultModel = DefaultModel;
	Storage->OnProjectSet().AddRaw(this, &Edge::OnNewProjectSet);

	EdgeEditor::Inspector InspectorWindow(Storage);
	EdgeEditor::SceneView SceneViewWindow(Storage);
	EdgeEditor::ProjectExplorer ProjectExplorerWindow(Storage);

	Razor::SceneSerializer::Deserialize(Engine.CurrentScene);
	while (!Engine.ShouldEngineClose())
	{
		std::shared_ptr<Razor::IRenderer>& Renderer = Engine.Renderer;

		// TODO this potentially doesn't need to get called here cause we might not be pressing play yet
		Engine.Step();

		const uint32_t SizeX = (uint32_t)ViewportSize.x;
		const uint32_t SizeY = (uint32_t)ViewportSize.y;

		PickBuffer->Refresh(SizeX, SizeY);
		SceneBuffer->Refresh(SizeX, SizeY);
		Renderer->SetViewport(0, 0, SizeX, SizeY);

		Renderer->BindFrameBuffer(PickBuffer->GetID());
		Renderer->ClearBuffer();
		Engine.RunRenderSystems(PickPipelineConfig);

		ProcessInput();

		Renderer->BindFrameBuffer(SceneBuffer->GetID());
		Renderer->ClearBuffer();
		Engine.RunRenderSystems(EditorPipelineConfig);

		Renderer->BindFrameBuffer();
		Renderer->ClearBuffer();

		Renderer->PollForEvents();

		// TODO runtime
		Engine.RunSystems();

		Engine.GetGUI().BeginNewFrame();
		CreateDockspace("Edge");
		InspectorWindow.Render();
		SceneViewWindow.Render();
		ProjectExplorerWindow.Render();
		RenderSceneViewport(SceneBuffer);
		ImGui::ShowMetricsWindow();
		ImGui::End();
		if (CurrentPopup)
		{
			if (CurrentPopup->Draw() == false)
			{
				delete(CurrentPopup);
				CurrentPopup = nullptr;
			}
		}
		Engine.GetGUI().EndFrame(Razor::Engine::Get().GetWindow(), Razor::Engine::Get().Renderer);
		Engine.Renderer->SwapBuffer(Razor::Engine::Get().GetWindow());
	}
	Razor::SceneSerializer::Serialize(Engine.CurrentScene);
	Razor::Engine::Get().Renderer->TerminateRendererAPI();
}

void Edge::RenderSceneViewport(Razor::Ref<Razor::Framebuffer> SceneBuffer)
{
	bool bIsOpen;
	ImGui::Begin("Scene", &bIsOpen, ImGuiWindowFlags_MenuBar || ImGuiWindowFlags_NoScrollbar);
	ViewportSize = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
	ImGui::Image(reinterpret_cast<void*>(SceneBuffer->GetTexture()), ImVec2(ViewportSize.x, ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
	ViewportPos = ImGui::GetItemRectMin();
	ImGui::End();
}

void Edge::ProcessInput()
{
	Razor::Engine::Get().ProcessInput();

	EditorCamera.ProcessInput(Razor::Engine::Get().GetDeltaTime());

	if (RazorIO::Get().GetStateForMouseButton(LEFT) == MOUSE_DOWN)
	{
		Vector2D MousePos = RazorIO::Get().CurrentMousePos;
		unsigned int OffsetMousePosX = MousePos.X - ViewportPos.x;
		unsigned int OffsetMousePosY = MousePos.Y - ViewportPos.y;

		if (OffsetMousePosX < 0 || OffsetMousePosX > PickBuffer->GetWidth() || OffsetMousePosY < 0 || OffsetMousePosY > PickBuffer->GetHeight())
		{
			return;
		}
		PickObject(ImVec2(OffsetMousePosX, OffsetMousePosY));
	}
}

void Edge::PickObject(ImVec2 MousePos)
{
	float Pixel[3];
	Razor::Engine::Get().GetRenderer()->ReadPixels(MousePos.x, PickBuffer->GetHeight() - MousePos.y, 1, 1, Pixel, PickBuffer->GetID());
	std::uint32_t PickedEntity = 0;
	PickedEntity = static_cast<std::uint32_t>(Pixel[0] * 255.0f) + (std::uint32_t(Pixel[1] * 255.0f) << 8)
			+ (std::uint32_t(Pixel[2] * 255.0f) << 16);
	Storage->SelectedEntity = Razor::Engine::Get().CurrentScene->GetEntity(entt::entity(PickedEntity));
}

void Edge::CreateDockspace(const std::string& Title)
{
	bool bIsOpen;
	bool bIsDockspaceOpen;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	ImGui::Begin(Title.c_str(), &bIsDockspaceOpen, window_flags);
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Create Project"))
			{
				CurrentPopup = new EdgeEditor::NewProjectPopupWindow();
			}
			if (ImGui::MenuItem("Open Project"))
			{
				CurrentPopup = new EdgeEditor::OpenProjectPopupWindow(Storage);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	ImGui::DockSpace(ImGui::GetID(Title.c_str()), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
}

template <>
struct fmt::formatter<Coral::String> : fmt::formatter<std::string> {
	auto format(const Coral::String& s, fmt::format_context& ctx) {
		// Use your std::string conversion operator
		return fmt::formatter<std::string>::format(std::string(s), ctx);
	}
};

void Edge::OnNewProjectSet()
{
	EdgeEditor::Project& LoadedProject = Storage->GetProject();
	RZ_INFO("Loading up project: " + LoadedProject.ProjectName);
	const std::string ProjectPath = "../Sandbox";
	//Load new scene
	Razor::Ref<Razor::Scene> MainScene = Razor::CreateRef<Razor::Scene>(ProjectPath + LoadedProject.MainScenePath);
	if (Razor::SceneSerializer::Deserialize(MainScene) == false)
	{
		LoadedProject.MainScenePath = "/assets/scenes/Main.rzscn";
		MainScene = Razor::CreateRef<Razor::Scene>(LoadedProject.MainScenePath);
		Razor::SceneSerializer::Serialize(MainScene);
		EdgeEditor::ProjectSerializer::Serialize("../", Razor::CreateRef<EdgeEditor::Project>(LoadedProject));
	}
	Razor::Engine::Get().CurrentScene = MainScene;
	//TODO we haven't dealt with tearing down an old scene and loading a new one yet that's mainly just destroying old entities
	//Load new dlls
	Razor::Ref<Coral::ManagedAssembly> ScriptBridgeAssembly = Razor::ScriptEngine::LoadAssembly(ProjectPath + "/" + LoadedProject.DllDirectory + "/" + "Razor-ScriptBridge.dll");
	Razor::Ref<Coral::ManagedAssembly> GameAssembly = Razor::ScriptEngine::LoadAssembly(ProjectPath + "/" + LoadedProject.DllDirectory + "/" + LoadedProject.ProjectName + ".dll");
	if (ScriptBridgeAssembly)
	{
		Razor::ScriptGlue::RegisterFunctions(*ScriptBridgeAssembly);
	}

	const std::vector<Coral::Type*> Types = GameAssembly->GetTypes();

	if (!ScriptBridgeAssembly->GetType("Razor.System"))
	{
		RZ_ERROR("Could not get System type");
	}

	for (Coral::Type* ScriptType : Types)
	{
		/*We are getting the type now however we are struggling to get base type as they are all null for some reason*/
		RZ_INFO("Script Type {0} and name {1}", ScriptType->GetTypeId(), ScriptType->GetFullName());
		RZ_INFO("Script Type Base Type {0} and name {1}", ScriptType->GetBaseType().GetTypeId(), ScriptType->GetBaseType().GetFullName());
		RZ_INFO("Razor System Type Id {0}", ScriptBridgeAssembly->GetType("Razor.System").GetTypeId());
		if (ScriptType->GetBaseType().GetTypeId() == ScriptBridgeAssembly->GetType("Razor.System").GetTypeId())
		{
			RZ_INFO("Instantiating system type");
			Coral::ManagedObject Instance = ScriptType->CreateInstance();
			Instance.InvokeMethod("Run", 1.0f);
		}
	}
}