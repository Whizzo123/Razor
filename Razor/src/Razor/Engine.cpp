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
#include <imgui.h>

namespace Razor
{

	Engine* Engine::GEngine = nullptr;

	
	glm::vec3 CameraDirection;

	void Engine::Init()
	{
		Renderer = std::make_shared<OpenGLRenderer>();
		Renderer->InitRendererAPI();

		std::shared_ptr<IWindowProvider> Provider = std::make_shared<OpenGLWindowProvider>();
		EngineWindow = std::make_unique<Window>(800, 600, Provider);
		
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
		ShaderTypeMap[typeid(DefaultMeshShader).name()] = D_MeshShader;
		std::shared_ptr<Shader> D_DebugShader = std::make_shared<DebugLightShader>();
		ShaderIDMap[D_DebugShader->ID] = D_DebugShader;
		ShaderTypeMap[typeid(DebugLightShader).name()] = D_DebugShader;
		std::shared_ptr<Shader> PickShader = std::make_shared<PickBufferShader>();
		ShaderIDMap[PickShader->ID] = PickShader;
		ShaderTypeMap[typeid(PickBufferShader).name()] = PickShader;

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

	void Engine::InitSystems()
	{
		Coordinator->InitSystems();
	}

	void Engine::Step()
	{
		float CurrentFrame = glfwGetTime();
		DeltaTime = CurrentFrame - LastFrame;
		LastFrame = CurrentFrame;
	}

	Model Engine::ProcessModel(const char* Path)
	{
		Model Tmp = Model();
		Tmp.LoadMesh(Path);
		return Tmp;
	}

	void Engine::ProcessInput()
	{
		if (RazorIO::Get().GetStateForKey(RazorKey::Escape) == KEY_PRESSED)
		{
			EngineWindow->SetWindowToClose();
		}
	}

	void Engine::Run()
	{
		
	}

	void Engine::PickObject(unsigned int PickBuffer)
	{
		if (RazorIO::Get().GetStateForMouseButton(LEFT) == MOUSE_DOWN)
		{
			Vector2D MousePos = RazorIO::Get().CurrentMousePos;
			unsigned char Pixel[4];
			Renderer->ReadPixels(MousePos.X, 600 - MousePos.Y, 1, 1, Pixel, PickBuffer);
			int PickedEntity = 0;
			PickedEntity = static_cast<int>(Pixel[0]) << 16 | static_cast<int>(Pixel[1]) << 8 | static_cast<int>(Pixel[2]);
			RZ_CORE_INFO("Picked {0}", PickedEntity);
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
		return ShaderTypeMap[Type];
	}
}