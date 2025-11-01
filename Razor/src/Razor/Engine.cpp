#include "Engine.h"
#include "MeshRenderer.h"
#include "Component.h"
#include "Renderer/OpenGLRenderer.h"
#include "Renderer/Model.h"
#include "Renderer/Shaders/DefaultMeshShader.h"
#include "Renderer/Shaders/DebugLightShader.h"
#include "Systems/RSMaterialPass.h"
#include "Systems/RSTransformationsPass.h"
#include "Systems/RSDirectionalLightingPass.h"
#include "Systems/RSCameraPass.h"
#include "Systems/RSRenderPass.h"
#include "../Platform/OpenGL/OpenGLIO.h"
#include "Systems/CollisionSystem.h"
#include "Systems/CameraController.h"
#include "Systems/RSPickBufferMaterialPass.h"
#include "Renderer/Shaders/PickBufferShader.h"
#include "Systems/RSPickBufferRenderPass.h"
#include "Systems/RSPointLightingPass.h"
#include "Systems/RSSpotLightingPass.h"
#include "../Platform/OpenGL/OpenGLWindowProvider.h"
#include "Scene/SceneSerializer.h"
#include "ImGui/RazorImGui.h"
#include "../Platform/OpenGL/GLFWTimeProvider.h"
#include "Window.h"
#include "Coordinator.h"
#include "Renderer/IRenderer.h"
#include "Renderer/Shaders/Shader.h"
#include "Scene/Scene.h"
#include "entt/entt.hpp"
#include "../Platform/Generic/ITimeProvider.h"
#include "Log.h"
#include "Scripting/ScriptInterface.h"
#include "Scene/Project.h"
#include "IO/File/ProjectSerializer.h"

namespace Razor
{

	Engine* Engine::GEngine = nullptr;

	
	glm::vec3 CameraDirection;

	void Engine::Init()
	{
		ScriptInterface = std::make_unique<Razor::ScriptInterface>();

		Renderer = std::make_shared<OpenGLRenderer>();
		Renderer->InitRendererAPI();

		std::shared_ptr<IWindowProvider> Provider = std::make_shared<OpenGLWindowProvider>();
		EngineWindow = std::make_unique<Window>(800, 600, Provider);

		TimeProvider = std::make_unique<GLFWTimeProvider>();
		
		Renderer->EnableDepthTesting(/*bEnable*/true);

		Coordinator = Coordinator::GetInstance();
		
		RazorGUI = std::make_unique<RazorImGui>();
		RazorGUI->Setup(EngineWindow->GetWindowProvider());
		RazorGUI->RegisterImGuiEvents();
		PlatformIO = std::make_unique<OpenGLIO>(std::dynamic_pointer_cast<OpenGLWindowProvider>(EngineWindow->GetWindowProvider())->GetPlatformWindowPtr());
		PlatformIO->RegisterInputCallbacks();
		
		

		//TODO don't like this being here
		std::shared_ptr<Shader> D_MeshShader = std::make_shared<DefaultMeshShader>();
		ShaderIDMap[D_MeshShader->ID] = D_MeshShader;
		ShaderTypeMap[std::string(typeid(DefaultMeshShader).name())] = D_MeshShader;
		std::shared_ptr<Shader> D_DebugShader = std::make_shared<DebugLightShader>();
		ShaderIDMap[D_DebugShader->ID] = D_DebugShader;
		ShaderTypeMap[std::string(typeid(DebugLightShader).name())] = D_DebugShader;
		std::shared_ptr<Shader> PickShader = std::make_shared<PickBufferShader>();
		ShaderIDMap[PickShader->ID] = PickShader;
		ShaderTypeMap[std::string(typeid(PickBufferShader).name())] = PickShader;

		CurrentScene = CreateRef<Scene>("Untitled.rzscn");

		SceneLights = std::make_shared<std::vector<Light*>>();
		// TODO rename mesh renderer doesn't do rendering just sets up the mesh for renderering
		Coordinator->RegisterSystem<MeshRenderer>(MeshRenderer(CurrentScene, Renderer, ShaderIDMap, SceneLights));
		Coordinator->RegisterSystem<CollisionSystem>(CollisionSystem(CurrentScene));
		Coordinator->RegisterSystem<CameraController>(CameraController(CurrentScene));

		//Render Systems
		Coordinator->RegisterSystem<RSMaterialPass>(RSMaterialPass(CurrentScene));
		Coordinator->RegisterSystem<RSTransformationsPass>(RSTransformationsPass(CurrentScene));
		Coordinator->RegisterSystem<RSDirectionalLightingPass>(RSDirectionalLightingPass(CurrentScene));
		Coordinator->RegisterSystem<RSCameraPass>(RSCameraPass(CurrentScene, Renderer));
		Coordinator->RegisterSystem<RSRenderPass>(RSRenderPass(CurrentScene, Renderer, ShaderIDMap));
		Coordinator->RegisterSystem<RSPickBufferMaterialPass>(RSPickBufferMaterialPass(CurrentScene));
		Coordinator->RegisterSystem<RSPickBufferRenderPass>(RSPickBufferRenderPass(CurrentScene, Renderer, ShaderIDMap));
		Coordinator->RegisterSystem<RSPointLightingPass>(RSPointLightingPass(CurrentScene));
		Coordinator->RegisterSystem<RSSpotLightingPass>(RSSpotLightingPass(CurrentScene));
		
	}

	Engine::Engine()
	{}

	Engine::~Engine()
	{
		RZ_CORE_INFO("Destroying razor");
		delete GEngine;
	}

	Engine& Engine::Get()
	{
		if (GEngine == nullptr)
		{
			GEngine = new Engine();
		}
		return *GEngine;
	}

	void Engine::InitSystems()
	{
		Coordinator->InitSystems();
	}

	void Engine::Step()
	{
		float CurrentFrame = TimeProvider->GetTime();
		DeltaTime = CurrentFrame - LastFrame;
		LastFrame = CurrentFrame;
	}

	Model Engine::ProcessModel(const char* Path)
	{
		Model Tmp = Model();
		Tmp.LoadMesh(Path);
		MeshRenderer::InitMesh(Tmp.GetModelMeshData());
		return Tmp;
	}

	void Engine::ProcessInput()
	{
		if (RazorIO::Get().GetStateForKey(RazorKey::Escape) == KEY_PRESSED)
		{
			EngineWindow->SetWindowToClose();
		}
	}

	void Engine::RenderImGui(uint64_t SceneTexture)
	{
		
	}

	// This is fine to have no checks as it would return 0 anyway if there was no shader for that ID meaning we always get a shader
	std::shared_ptr<Shader> Engine::GetShaderForID(uint8_t ID)
	{
		return ShaderIDMap[ID];
	}

	std::shared_ptr<Shader> Engine::GetShaderForType(const char* Type)
	{
		return ShaderTypeMap[std::string(Type)];
	}

	void Engine::RunRenderSystems(const RenderPipelineConfig& Config) 
	{
		Coordinator->RunRenderSystems(Config); 
	}

	void Engine::RunSystems() 
	{ 
		Coordinator->RunSystems(DeltaTime); 
	}

	std::shared_ptr<Coordinator> Engine::GetCoordinator()
	{
		return Coordinator;
	}

	bool Engine::ShouldEngineClose() 
	{ 
		return EngineWindow->ShouldWindowClose(); 
	}

	Window& Engine::GetWindow() 
	{ 
		return *EngineWindow; 
	}

	std::shared_ptr<IRenderer> Engine::GetRenderer()
	{
		return Renderer;
	}

	ScriptInterface& Engine::GetScriptInterface()
	{
		return *ScriptInterface;
	}

	void Engine::LoadProject(const std::string& ProjectPath)
	{
		if (ProjectPath.empty())
		{
			RZ_CORE_ERROR("Project path is empty");
			return;
		}

		if (LoadedProject == nullptr)
		{
			LoadedProject = CreateRef<Project>();
		}

		ProjectSerializer::Deserialize(ProjectPath, LoadedProject);

		RZ_CORE_INFO("Loading up project: " + LoadedProject->ProjectName);
		const std::string Path = "../Sandbox";
		//Load new scene
		Ref<Scene> MainScene = CreateRef<Scene>(Path + LoadedProject->MainScenePath);
		if (SceneSerializer::Deserialize(MainScene) == false)
		{
			LoadedProject->MainScenePath = "/assets/scenes/Main.rzscn";
			MainScene = CreateRef<Scene>(LoadedProject->MainScenePath);
			SceneSerializer::Serialize(MainScene);
			ProjectSerializer::Serialize("../", LoadedProject);
		}
		CurrentScene = MainScene;
		//TODO we haven't dealt with tearing down an old scene and loading a new one yet that's mainly just destroying old entities
		//Load new dlls
		// We need to move all Coral code into Razor behind an interface through which we will ask Coral things as we seem to be losing all of our function ptrs to the managed library over the DLL boundary
		BridgeAssembly = CreateScope<ScriptAssembly>(ScriptInterface->LoadAssembly(Path + "/" + LoadedProject->DllDirectory + "/" + "Razor-ScriptBridge.dll", true));
		GameAssembly = CreateScope<ScriptAssembly>(ScriptInterface->LoadAssembly(Path + "/" + LoadedProject->DllDirectory + "/" + LoadedProject->ProjectName + ".dll", false));

		std::vector<ScriptType> Types = ScriptInterface->GetTypes(*(GameAssembly.get()));

		if (!ScriptInterface->GetType(*(BridgeAssembly.get()), "Razor.System"))
		{
			RZ_CORE_ERROR("Could not get System type");
		}

		for (Razor::ScriptType ScriptType : Types)
		{
			/*We are getting the type now however we are struggling to get base type as they are all null for some reason*/
			RZ_CORE_INFO("Script Type {0} and name {1}", ScriptType.id, ScriptType.fullName);
			RZ_CORE_INFO("Script Type Base Type {0} and name {1}", ScriptInterface->GetBaseType(ScriptType).id, ScriptInterface->GetBaseType(ScriptType).fullName);
			RZ_CORE_INFO("Razor System Type Id {0}", ScriptInterface->GetType(*(BridgeAssembly.get()), "Razor.System").id);
			if (ScriptInterface->GetBaseType(ScriptType).id == ScriptInterface->GetType(*(BridgeAssembly.get()), "Razor.System").id)
			{
				RZ_INFO("Instantiating system type");
				Razor::ScriptObject TestObject = ScriptInterface->CreateInstance(ScriptType);
				ScriptInterface->InvokeMethod(TestObject, "Run", 1.0f);
			}
		}
	}
}