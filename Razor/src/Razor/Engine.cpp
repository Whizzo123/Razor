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

		// Seperate into some sort of all component/system registration
		//Swap out Coordinator here with Scene and we will fix this later one :)
		// Move this to edge as well perhaps

		CurrentScene = CreateRef<Scene>("Untitled.rzscn");

		ComponentType MeshType = Coordinator->GetComponentType<Mesh>();
		ComponentType MaterialType = Coordinator->GetComponentType<Material>();
		ComponentType TransformType = Coordinator->GetComponentType<Transform>();

		SceneLights = std::make_shared<std::vector<Light*>>();
		ComponentType MeshRendererSignature[3]{ MeshType, MaterialType, TransformType };
		// TODO rename mesh renderer doesn't do rendering just sets up the mesh for renderering
		Coordinator->RegisterSystem<MeshRenderer>(MeshRenderer(Renderer, ShaderIDMap, SceneLights), MeshRendererSignature, 3);
		ComponentType CollisionSystemSignature[2]{ TransformType, Coordinator->GetComponentType<Collider>() };
		Coordinator->RegisterSystem<CollisionSystem>(CollisionSystem(), CollisionSystemSignature, 2);
		ComponentType CameraControllerSignature[1]{ Coordinator->GetComponentType<Camera>() };
		Coordinator->RegisterSystem<CameraController>(CameraController(), CameraControllerSignature, 1);

		//Render Systems
		
		ComponentType MaterialPassSignature[1] { MaterialType };
		Coordinator->RegisterSystem<RSMaterialPass>(RSMaterialPass(), MaterialPassSignature, 1);
		ComponentType TransformPassSignature[1]{ Coordinator->GetComponentType<Transform>() };
		Coordinator->RegisterSystem<RSTransformationsPass>(RSTransformationsPass(), TransformPassSignature, 1);
		ComponentType DirectionalLightPassSignature[1]{ Coordinator->GetComponentType<DirectionalLight>() };
		Coordinator->RegisterSystem<RSDirectionalLightingPass>(RSDirectionalLightingPass(), DirectionalLightPassSignature, 1);
		ComponentType CameraPassSignature[1]{ Coordinator->GetComponentType<Camera>() };
		Coordinator->RegisterSystem<RSCameraPass>(RSCameraPass(Renderer), CameraPassSignature, 1);
		ComponentType RenderPassSignature[2]{ MeshType, MaterialType };
		Coordinator->RegisterSystem<RSRenderPass>(RSRenderPass(Renderer, ShaderIDMap), RenderPassSignature, 2);
		ComponentType PickBufferMaterialPassSignature[2]{ MeshType, MaterialType };
		Coordinator->RegisterSystem<RSPickBufferMaterialPass>(RSPickBufferMaterialPass(), PickBufferMaterialPassSignature, 2);
		ComponentType PickBufferRenderPassSignature[2]{ MeshType, MaterialType };
		Coordinator->RegisterSystem<RSPickBufferRenderPass>(RSPickBufferRenderPass(Renderer, ShaderIDMap), PickBufferRenderPassSignature, 2);
		ComponentType PointLightingPassSignature[2]{ Coordinator->GetComponentType<PointLight>() };
		Coordinator->RegisterSystem<RSPointLightingPass>(RSPointLightingPass(), PointLightingPassSignature, 2);
		ComponentType SpotLightingPassSignature[1]{ Coordinator->GetComponentType<SpotLight>() };
		Coordinator->RegisterSystem<RSSpotLightingPass>(RSSpotLightingPass(), SpotLightingPassSignature, 1);
		
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

	ModelInfo Engine::ProcessModel(const char* Path)
	{
		Model Tmp = Model();
		return Tmp.LoadMesh(Path);
	}

	void Engine::ProcessInput()
	{
		if (RazorIO::Get().GetStateForKey(RazorKey::Escape) == KEY_PRESSED)
		{
			EngineWindow->SetWindowToClose();
		}
	}

	Entity Engine::CreateEntity()
	{
		return Coordinator->CreateEntity();
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