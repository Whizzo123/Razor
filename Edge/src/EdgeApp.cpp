#include <Razor.h>
#include <Razor/EntryPoint.h>
#include <algorithm>

#include "Gui/IEditorWindow.h"
#include "Gui/SceneWindow.h"
#include "Gui/GameWindow.h"
#include "Inspector.h"
#include "SceneView.h"
#include "EditorStorage.h"
#include "ProjectExplorer.h"
#include "Systems/RSEditorCamera.h"
#include "EditorCamera.h"
#include "Gui/NewProjectPopupWindow.h"
#include "Gui/OpenProjectPopupWindow.h"
#include "SystemView.h"
#include "Gui/IEditorWindow.h"
#include <Razor/Systems/RSCameraPass.h>
#include <unordered_map>



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
	std::unordered_map<EdgeEditor::EditorWindowName, Razor::Scope<EdgeEditor::IEditorWindow>> _mEditorWindows;
	EdgeEditor::PopupWindow* CurrentPopup = nullptr; /** Holds current popup open */

private:
	Razor::Ref<EdgeEditor::EditorStorage> Storage; /** Container object to hold data to be shared among windows*/
	Razor::Ref<Razor::Scene> _mPlaybackSceneBackup;
	
};

Razor::Application* Razor::CreateApplication()
{
	return new Edge();
}

void Edge::Run()
{
	
	Razor::Engine& Engine = Razor::Engine::Get();

	Engine.InitSystems();

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

	_mEditorWindows.insert({EdgeEditor::EditorWindowName::INSPECTOR, Razor::CreateScope<EdgeEditor::Inspector>(Storage)});
	_mEditorWindows.insert({EdgeEditor::EditorWindowName::SCENEVIEW, Razor::CreateScope<EdgeEditor::SceneView>(Storage)});
	_mEditorWindows.insert({EdgeEditor::EditorWindowName::PROJECTEXPLORER, Razor::CreateScope<EdgeEditor::ProjectExplorer>(Storage)});
	_mEditorWindows.insert({EdgeEditor::EditorWindowName::SYSTEMVIEW,  Razor::CreateScope<EdgeEditor::SystemView>()});
	_mEditorWindows.insert({EdgeEditor::EditorWindowName::SCENEWINDOW, Razor::CreateScope<EdgeEditor::SceneWindow>(Storage)});
	_mEditorWindows.insert({EdgeEditor::EditorWindowName::GAMEWINDOW, Razor::CreateScope<EdgeEditor::GameWindow>(Storage)});

	Razor::SceneSerializer::Deserialize(Engine.mCurrentScene);
	while (!Engine.ShouldEngineClose())
	{
		ProcessInput();

		Engine.GetGUI().BeginNewFrame();
		CreateDockspace("Edge");
		for (const auto& [name, window] : _mEditorWindows)
		{
			window->Render();
		}

		Engine.mRenderer->PollForEvents();

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

void Edge::ProcessInput()
{
	Razor::Engine::Get().ProcessInput();
	for (const auto& [name, window] : _mEditorWindows)
	{
		window->ProcessInput();
	}
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
		if (_mEditorWindows.contains(EdgeEditor::EditorWindowName::PROJECTEXPLORER))
		{
			EdgeEditor::IEditorWindow* window = _mEditorWindows[EdgeEditor::EditorWindowName::PROJECTEXPLORER].get();
			EdgeEditor::ProjectExplorer* explorer = dynamic_cast<EdgeEditor::ProjectExplorer*>(window);
			explorer->Refresh(Engine.GetAssetDirectory()->GetRootFolder());
		}
	}
}