#include "Engine.h"
#include "Component.h"
#include "Renderer/OpenGLRenderer.h"
#include "Renderer/Model.h"
#include "Renderer/Shaders/DefaultMeshShader.h"
#include "Renderer/Shaders/DebugLightShader.h"
#include "Renderer/Shaders/DefaultTextShader.h"
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
#include "Systems/RSTextPass.h"
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
#include "Core/Entity.h"
#include <Coral/ManagedObject.hpp>
#include "Assets/AssetDirectory.h"
#include "Physics/JoltPhysics/JoltPhysicsEngine.h"
#include "Physics/JoltPhysics/JoltDebugRenderer.h"
#include "Systems/PhysicsSystem.h"
#include "Renderer/Font/FontLoader.h"

namespace Razor
{

	Scope<Engine> Engine::_mGEngine = nullptr;

	void Engine::Init()
	{
		_mScriptInterface = CreateScope<Razor::ScriptInterface>();

		mRenderer = CreateRef<OpenGLRenderer>();
		mRenderer->InitRendererAPI();

		Ref<IWindowProvider> Provider = CreateRef<OpenGLWindowProvider>();
		_mEngineWindow = CreateScope<Window>(800, 600, Provider);

		_mTimeProvider = CreateScope<GLFWTimeProvider>();
		
		mRenderer->EnableDepthTesting(/*bEnable*/true);

		_mCoordinator = Coordinator::GetInstance();
		
		// Platform must be called before RazorGUI so ImGui chains our renderer input callbacks in 
		_mPlatformIO = CreateScope<OpenGLIO>(std::dynamic_pointer_cast<OpenGLWindowProvider>(_mEngineWindow->GetWindowProvider())->GetPlatformWindowPtr());
		_mPlatformIO->RegisterInputCallbacks();
		_mRazorGUI = CreateScope<RazorImGui>();
		_mRazorGUI->Setup(_mEngineWindow->GetWindowProvider());

		_mDebugDrawBuffer = CreateScope<PhysicsDebugDrawBuffer>();
		_mPhysicsDebugRenderer = CreateRef<JoltDebugRenderer>(_mDebugDrawBuffer.get());
		_mPhysicsEngine = CreateScope<JoltPhysicsEngine>(std::dynamic_pointer_cast<JoltDebugRenderer>(_mPhysicsDebugRenderer));
		
		

		//TODO don't like this being here
		Ref<Shader> D_MeshShader = CreateRef<DefaultMeshShader>();
		_mShaderIDMap[D_MeshShader->ID] = D_MeshShader;
		_mShaderTypeMap[std::string(typeid(DefaultMeshShader).name())] = D_MeshShader;
		Ref<Shader> D_DebugShader = CreateRef<DebugLightShader>();
		_mShaderIDMap[D_DebugShader->ID] = D_DebugShader;
		_mShaderTypeMap[std::string(typeid(DebugLightShader).name())] = D_DebugShader;
		Ref<Shader> PickShader = CreateRef<PickBufferShader>();
		_mShaderIDMap[PickShader->ID] = PickShader;
		_mShaderTypeMap[std::string(typeid(PickBufferShader).name())] = PickShader;
		Ref<Shader> TextShader = CreateRef<DefaultTextShader>();
		_mShaderIDMap[TextShader->ID] = TextShader;
		_mShaderTypeMap[std::string(typeid(DefaultTextShader).name())] = TextShader;

		mCurrentScene = CreateRef<Scene>("Untitled.rzscn");

		_mSceneLights = CreateRef<std::vector<Light*>>();
		// TODO rename mesh renderer doesn't do rendering just sets up the mesh for renderering
		_mCoordinator->RegisterSystem<CollisionSystem>(CollisionSystem(mCurrentScene));
		_mCoordinator->RegisterSystem<CameraController>(CameraController(mCurrentScene));
		_mCoordinator->RegisterSystem<PhysicsSystem>(mCurrentScene);
		
		//Render Systems
		_mCoordinator->RegisterSystem<RSMaterialPass>(RSMaterialPass(mCurrentScene));
		_mCoordinator->RegisterSystem<RSTransformationsPass>(RSTransformationsPass(mCurrentScene));
		_mCoordinator->RegisterSystem<RSDirectionalLightingPass>(RSDirectionalLightingPass(mCurrentScene));
		_mCameraPass = _mCoordinator->RegisterSystem<RSCameraPass>(RSCameraPass(mCurrentScene, mRenderer));
		_mCoordinator->RegisterSystem<RSRenderPass>(RSRenderPass(mCurrentScene, mRenderer, _mShaderIDMap, _mShaderTypeMap));
		_mCoordinator->RegisterSystem<RSPickBufferMaterialPass>(RSPickBufferMaterialPass(mCurrentScene));
		_mCoordinator->RegisterSystem<RSPickBufferRenderPass>(RSPickBufferRenderPass(mCurrentScene, mRenderer, _mShaderIDMap));
		_mCoordinator->RegisterSystem<RSPointLightingPass>(RSPointLightingPass(mCurrentScene));
		_mCoordinator->RegisterSystem<RSSpotLightingPass>(RSSpotLightingPass(mCurrentScene));
		_mCoordinator->RegisterSystem<RSTextPass>(RSTextPass(mCurrentScene));
		
		_mFontLoader = CreateScope<FontLoader>();
	}

	Engine::Engine()
	{}

	Engine::~Engine()
	{
		
	}

	Engine& Engine::Get()
	{
		if (_mGEngine == nullptr)
		{
			_mGEngine = Scope<Engine>(new Engine());
		}
		return *_mGEngine;
	}

	void Engine::InitSystems()
	{
		_mCoordinator->InitSystems();
	}

	void Engine::Step()
	{
		float currentFrame = _mTimeProvider->GetTime();
		if(_mLastFrame == 0.0f)
		{
			_mLastFrame = _mTimeProvider->GetTime();
		}
		_mDeltaTime = currentFrame - _mLastFrame;
		_mLastFrame = currentFrame;
	}

	void Engine::ProcessInput()
	{
		if (RazorIO::Get().GetStateForKey(RazorKey::Escape) == KEY_PRESSED)
		{
			_mEngineWindow->SetWindowToClose();
		}
	}

	// This is fine to have no checks as it would return 0 anyway if there was no shader for that ID meaning we always get a shader
	std::shared_ptr<Shader> Engine::GetShaderForID(uint8_t id)
	{
		return _mShaderIDMap[id];
	}

	std::shared_ptr<Shader> Engine::GetShaderForType(const char* type)
	{
		return _mShaderTypeMap[std::string(type)];
	}

	void Engine::Render(int32_t targetId, const RenderPipelineConfig& config) 
	{
		mRenderer->BindFrameBuffer(targetId);
		mRenderer->ClearBuffer();
		_mCoordinator->RunRenderSystems(config); 
		mRenderer->BindFrameBuffer();
		mRenderer->ClearBuffer();
	}

	void Engine::RunSystems() 
	{ 
		_mCoordinator->RunSystems(_mDeltaTime);
		mCurrentScene->RunSystems(_mDeltaTime);
	}

	std::shared_ptr<Coordinator> Engine::GetCoordinator()
	{
		return _mCoordinator;
	}

	bool Engine::ShouldEngineClose() 
	{ 
		return _mEngineWindow->ShouldWindowClose(); 
	}

	Window& Engine::GetWindow() 
	{ 
		return *_mEngineWindow; 
	}

	std::shared_ptr<IRenderer> Engine::GetRenderer()
	{
		return mRenderer;
	}

	ScriptInterface& Engine::GetScriptInterface()
	{
		return *_mScriptInterface;
	}

	void Engine::SaveProject()
	{
		if (_mLoadedProject == nullptr)
		{
			RZ_CORE_ERROR("Attempting to save project when no project is loaded");
			return;
		}

		ProjectSerializer::Serialize(_mLoadedProject->m_ProjectPath, _mLoadedProject);
		SceneSerializer::Serialize(mCurrentScene);
	}

	void Engine::CreateProject()
	{
		//TODO need to link up how the save is gonna work for this as they have no paths setup
		_mLoadedProject = CreateRef<Project>();
		mCurrentScene = CreateRef<Scene>("");
	}


	void Engine::LoadProject(const std::string& projectPath)
	{
		if (projectPath.empty())
		{
			RZ_CORE_ERROR("Project path is empty");
			return;
		}

		if (_mLoadedProject == nullptr)
		{
			_mLoadedProject = CreateRef<Project>();
		}

		const std::string path = "Sandbox";

		ProjectSerializer::Deserialize(projectPath, _mLoadedProject);
		RZ_CORE_INFO("Loading up project: {0}", _mLoadedProject->m_ProjectName);
		// TODO move assembly holding into ScriptEngine
		_mBridgeAssembly = _mScriptInterface->LoadAssembly(path + "/" + _mLoadedProject->m_DllDirectory + "/" + "Razor-ScriptBridge.dll", true);
		_mGameAssembly = _mScriptInterface->LoadAssembly(path + "/" + _mLoadedProject->m_DllDirectory + "/" + _mLoadedProject->m_ProjectName + ".dll", false);

		// Load main scene
		Ref<Scene> mainScene = CreateRef<Scene>(path + _mLoadedProject->m_MainScenePath);
		if (SceneSerializer::Deserialize(mainScene) == false)
		{
			_mLoadedProject->m_MainScenePath = "/assets/scenes/Main.rzscn";
			mainScene = CreateRef<Scene>(_mLoadedProject->m_MainScenePath);
			SceneSerializer::Serialize(mainScene);
			ProjectSerializer::Serialize("../", _mLoadedProject);
		}
		*mCurrentScene = std::move(*mainScene);
		_mAssetDirectory = CreateRef<AssetDirectory>(path + "/" + _mLoadedProject->m_AssetDirectory, mRenderer);
	}

	void Engine::SetGameCameraViewportSize(uint32_t w, uint32_t h)
	{
		if (_mCameraPass)
			_mCameraPass->SetViewportSize(w, h);
	}

	void Engine::RuntimeStart()
	{
		if(_mbIsRuntimeRunning.load())
		{
			RZ_CORE_WARN("Runtime is already running");
			return;
		}
		RZ_CORE_INFO("Starting Runtime");
		_mbIsRuntimeRunning.store(true);
		mCurrentScene->StartScene();
		_mRuntimeThread = std::thread(&Engine::RunRuntime, this);
	}

	void Engine::RunRuntime()
	{
		while (_mbIsRuntimeRunning)
		{
			Step();
			_mPhysicsEngine->Simulate(_mDeltaTime);
			RunSystems();
		}
		_mLastFrame = 0.0f;
		RZ_CORE_INFO("Exiting Runtime Thread");
	}

	void Engine::RuntimeStop()
	{
		RZ_CORE_INFO("Stopping Runtime");
		_mbIsRuntimeRunning.store(false);
		if(_mRuntimeThread.joinable())
		{
			_mRuntimeThread.join();
			mCurrentScene->StopScene();
		}
	}

	Ref<AssetDirectory> Engine::GetAssetDirectory()
	{
		return _mAssetDirectory;
	}

	IPhysicsEngine& Engine::GetPhysicsEngine()
	{
		return *_mPhysicsEngine;
	}

	void Engine::PopulateRenderPipelineDebugData()
	{
		std::unique_lock lock(_mDebugDrawBuffer->mutex, std::try_to_lock);
		if (!lock.owns_lock())
			return;
		_mCoordinator->SetRenderPipelineDebugData(_mDebugDrawBuffer->lines, _mDebugDrawBuffer->triangles);
	}

	void Engine::ClearDebugDrawBuffer()
	{
		std::scoped_lock lock(_mDebugDrawBuffer->mutex);
		_mDebugDrawBuffer->lines.clear();
		_mDebugDrawBuffer->triangles.clear();
	}

	FontLoader& Engine::GetFontLoader()
	{
		return *_mFontLoader;
	}
}