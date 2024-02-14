#include "Engine.h"
#include "MeshRenderer.h"
#include "Component.h"
#include "Renderer/OpenGLRenderer.h"
#include "Renderer/Model.h"
#include "Systems/LightRenderer.h"
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

namespace Razor
{

	Engine* Engine::GEngine = nullptr;

	
	glm::vec3 CameraDirection;

	void Engine::Init()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		window = std::make_unique<Window>(800, 600);
		glEnable(GL_DEPTH_TEST);
		Coordinator = Coordinator::GetInstance();
		Renderer = std::make_shared<OpenGLRenderer>();
		//glfwSetInputMode(GEngine->window->GetWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		RazorGUI = std::make_unique<RazorImGui>();
		RazorGUI->Setup(window->GetWindowPtr());
		RazorGUI->RegisterImGuiEvents();
		PlatformIO = std::make_unique<OpenGLIO>(window->GetWindowPtr());
		PlatformIO->RegisterInputCallbacks();
		//CREATE SHADERS
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
		Coordinator->RegisterComponent<Mesh>();
		Coordinator->RegisterComponent<Material>();
		Coordinator->RegisterComponent<Transform>();
		Coordinator->RegisterComponent<Collider>();
		Coordinator->RegisterComponent<Light>();
		Coordinator->RegisterComponent<DirectionalLight>();
		Coordinator->RegisterComponent<Camera>();

		SceneLights = std::make_shared<std::vector<Light*>>();
		Coordinator->RegisterSystem<MeshRenderer>(MeshRenderer(Renderer, ShaderIDMap, SceneLights));
		Coordinator->RegisterSystem<LightRenderer>(LightRenderer(Renderer, SceneLights));
		Coordinator->RegisterSystem<CollisionSystem>(CollisionSystem());
		Coordinator->RegisterSystem<CameraController>(CameraController());

		Signature sig;
		sig.set(Coordinator->GetComponentType<Mesh>());
		sig.set(Coordinator->GetComponentType<Material>());
		sig.set(Coordinator->GetComponentType<Transform>());
		Signature LightSig;
		LightSig.set(Coordinator->GetComponentType<DirectionalLight>());
		Signature ColliderSig;
		ColliderSig.set(Coordinator->GetComponentType<Transform>());
		ColliderSig.set(Coordinator->GetComponentType<Collider>());
		Signature CameraControllerSig;
		CameraControllerSig.set(Coordinator->GetComponentType<Camera>());
		
		Coordinator->SetSystemSignature<LightRenderer>(LightSig);
		Coordinator->SetSystemSignature<CollisionSystem>(ColliderSig);
		Coordinator->SetSystemSignature<MeshRenderer>(sig);
		Coordinator->SetSystemSignature<CameraController>(CameraControllerSig);

		//Render Systems
		Coordinator->RegisterSystem<RSMaterialPass>(RSMaterialPass());
		Signature RSMaterialPassSig;
		RSMaterialPassSig.set(Coordinator->GetComponentType<Material>());
		Coordinator->SetSystemSignature<RSMaterialPass>(RSMaterialPassSig);
		Coordinator->RegisterSystem<RSTransformationsPass>(RSTransformationsPass());
		Signature RSTransformationsPassSig;
		RSTransformationsPassSig.set(Coordinator->GetComponentType<Transform>());
		Coordinator->SetSystemSignature<RSTransformationsPass>(RSTransformationsPassSig);
		Coordinator->RegisterSystem<RSDirectionalLightingPass>(RSDirectionalLightingPass());
		Signature RSDirectionalLightingPassSig;
		RSDirectionalLightingPassSig.set(Coordinator->GetComponentType<DirectionalLight>());
		Coordinator->SetSystemSignature<RSDirectionalLightingPass>(RSDirectionalLightingPassSig);
		Coordinator->RegisterSystem<RSCameraPass>(RSCameraPass(Renderer));
		Signature RSCameraPassSig;
		RSCameraPassSig.set(Coordinator->GetComponentType<Camera>());
		Coordinator->SetSystemSignature<RSCameraPass>(RSCameraPassSig);
		Coordinator->RegisterSystem<RSRenderPass>(RSRenderPass(Renderer, ShaderIDMap));
		Signature RSRenderPassSig;
		RSRenderPassSig.set(Coordinator->GetComponentType<Mesh>());
		RSRenderPassSig.set(Coordinator->GetComponentType<Material>());
		Coordinator->SetSystemSignature<RSRenderPass>(RSRenderPassSig);
		Coordinator->RegisterSystem<RSPickBufferMaterialPass>(RSPickBufferMaterialPass());
		Signature RSPickBufferMaterialPassSig;
		RSPickBufferMaterialPassSig.set(Coordinator->GetComponentType<Material>());
		Coordinator->SetSystemSignature<RSPickBufferMaterialPass>(RSPickBufferMaterialPassSig);
		Coordinator->RegisterSystem<RSPickBufferRenderPass>(RSPickBufferRenderPass(Renderer, ShaderIDMap));
		Signature RSPickBufferRenderPassSig;
		// Need this to work for all visible types
		RSPickBufferRenderPassSig.set(Coordinator->GetComponentType<Mesh>());
		RSPickBufferRenderPassSig.set(Coordinator->GetComponentType<Material>());
		Coordinator->SetSystemSignature<RSPickBufferRenderPass>(RSPickBufferRenderPassSig);

	}

	ModelInfo Engine::ProcessModel(const char* Path)
	{
		Model Tmp = Model();
		return Tmp.LoadMesh(Path);
	}

	void Engine::ProcessInput(GLFWwindow* window)
	{
		if (RazorIO::Get().GetStateForKey(RazorKey::Escape) == KEY_PRESSED)
		{
			glfwSetWindowShouldClose(window, true);
		}
	}

	Entity Engine::CreateEntity()
	{
		return Coordinator->CreateEntity();
	}
	// Also just leaving this here we are doing the two render function for detecting what's selected so each entity rendered in framebuffer will have different colour
	// And we will sample from that called GPU picking

	void Engine::Run()
	{
		RenderPipelineConfig PipelineConfig;
		PipelineConfig.Configuration.push_back(RenderStageConfig{ RenderStage::RENDER_STAGE_MATERIAL_PASS, std::vector<const char*> { typeid(RSMaterialPass).name() } });
		PipelineConfig.Configuration.push_back(RenderStageConfig{ RenderStage::RENDER_STAGE_LIGHTING_PASS, std::vector<const char*> { typeid(RSDirectionalLightingPass).name() } });
		PipelineConfig.Configuration.push_back(RenderStageConfig{ RenderStage::RENDER_STAGE_TRANSFORMATION_PASS, std::vector<const char*> { typeid(RSTransformationsPass).name() } });
		PipelineConfig.Configuration.push_back(RenderStageConfig{ RenderStage::RENDER_STAGE_CAMERA_PASS, std::vector<const char*> { typeid(RSCameraPass).name() } });
		PipelineConfig.Configuration.push_back(RenderStageConfig{ RenderStage::RENDER_STAGE_RENDER, std::vector<const char*> { typeid(RSRenderPass).name() } });

		RenderPipelineConfig PickPipelineConfig;
		PickPipelineConfig.Configuration.push_back(RenderStageConfig{ RenderStage::RENDER_STAGE_MATERIAL_PASS, std::vector<const char*> { typeid(RSPickBufferMaterialPass).name() } });
		PickPipelineConfig.Configuration.push_back(RenderStageConfig{ RenderStage::RENDER_STAGE_LIGHTING_PASS, std::vector<const char*> { typeid(RSDirectionalLightingPass).name() } });
		PickPipelineConfig.Configuration.push_back(RenderStageConfig{ RenderStage::RENDER_STAGE_TRANSFORMATION_PASS, std::vector<const char*> { typeid(RSTransformationsPass).name() } });
		PickPipelineConfig.Configuration.push_back(RenderStageConfig{ RenderStage::RENDER_STAGE_CAMERA_PASS, std::vector<const char*> { typeid(RSCameraPass).name() } });
		PickPipelineConfig.Configuration.push_back(RenderStageConfig{ RenderStage::RENDER_STAGE_RENDER, std::vector<const char*> { typeid(RSPickBufferRenderPass).name() } });
		unsigned int PickBuffer = Renderer->CreateFrameBuffer();
		unsigned int PickBufferTexture = Renderer->CreateTextureForFrameBuffer(PickBuffer);
		Coordinator->InitSystems();
		while (!glfwWindowShouldClose(window->GetWindowPtr()))
		{
			float CurrentFrame = glfwGetTime();
			DeltaTime = CurrentFrame - LastFrame;
			LastFrame = CurrentFrame;

			ProcessInput(window->GetWindowPtr());

			Coordinator->RunSystems(DeltaTime);

			// Render to pick buffer - this is not performant :) 
			Renderer->BindFrameBuffer(PickBuffer);
			Renderer->ClearBuffer();
			Coordinator->RunRenderSystems(PickPipelineConfig);
			PickObject(PickBuffer);
			// True render
			Renderer->BindFrameBuffer();
			Renderer->ClearBuffer();
			Coordinator->RunRenderSystems(PipelineConfig);
			
			

			glfwPollEvents(); 
			RazorGUI->Render(*std::move(window));
			Renderer->SwapBuffer(*window);
		}
		glfwTerminate();
	}

	void Engine::PickObject(unsigned int PickBuffer)
	{
		if (RazorIO::Get().GetStateForMouseButton(LEFT) == MOUSE_DOWN)
		{
			Vector2D MousePos = RazorIO::Get().CurrentMousePos;
			unsigned char Pixel[4];
			// Don't think we are decoding this correctly or decoding correctly either
			Renderer->ReadPixels(MousePos.X, 600 - MousePos.Y, 1, 1, Pixel, PickBuffer);
			int PickedEntity = 0;
			// Not quite grabbing values back right yet but storing okay i think :)
			PickedEntity = static_cast<int>(Pixel[0]) << 16 | static_cast<int>(Pixel[1]) << 8 | static_cast<int>(Pixel[2]);
			RZ_CORE_INFO("Picked {0}", PickedEntity);
		}
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