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

namespace Razor
{

	Engine* Engine::GEngine = nullptr;

	float MouseLastX = 400;
	float MouseLastY = 300;
	float Yaw = -90.0f;
	float Pitch = 0.0f;
	bool bIsFirstMouse;
	glm::vec3 CameraDirection;

	void Mouse_Callback(GLFWwindow* window, double Xpos, double Ypos)
	{
		if (bIsFirstMouse)
		{
			MouseLastX = Xpos;
			MouseLastY = Ypos;
			bIsFirstMouse = false;
		}

		float Xoffset = Xpos - MouseLastX;
		float Yoffset = Ypos - MouseLastY;
		MouseLastX = Xpos;
		MouseLastY = Ypos;

		const float MouseSensitivity = 0.1f;
		Xoffset *= MouseSensitivity;
		Yoffset *= MouseSensitivity;

		Yaw += Xoffset;
		Pitch += Yoffset;

		Pitch = Pitch > 89.0f ? 89.0f : ((Pitch < -89.0f) ? -89.0f : Pitch);

		CameraDirection.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		CameraDirection.y = sin(glm::radians(Pitch));
		CameraDirection.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	}

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
		glfwSetCursorPosCallback(window->GetWindowPtr(), Mouse_Callback);
		RazorGUI = std::make_unique<RazorImGui>();
		RazorGUI->Setup();
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

		//Seperate this into a camera component and camera system
		Renderer->RendererCamera.CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		Renderer->RendererCamera.CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		Renderer->RendererCamera.CameraDirection = glm::normalize(Renderer->RendererCamera.CameraPos - Renderer->RendererCamera.CameraTarget);

		Renderer->RendererCamera.Up = glm::vec3(0.0f, 1.0f, 0.0f);
		Renderer->RendererCamera.CameraRight = glm::normalize(glm::cross(Renderer->RendererCamera.Up, Renderer->RendererCamera.CameraDirection));
		Renderer->RendererCamera.CameraUp = glm::cross(Renderer->RendererCamera.CameraDirection, Renderer->RendererCamera.CameraRight);
		Renderer->RendererCamera.CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

		// Seperate into some sort of all component/system registration
		Coordinator->RegisterComponent<Mesh>();
		Coordinator->RegisterComponent<Material>();
		Coordinator->RegisterComponent<Transform>();
		SceneLights = std::make_shared<std::vector<Light*>>();
		Coordinator->RegisterSystem<MeshRenderer>(MeshRenderer(Renderer, ShaderIDMap, SceneLights));
		Signature sig;
		sig.set(Coordinator->GetComponentType<Mesh>());
		sig.set(Coordinator->GetComponentType<Material>());
		sig.set(Coordinator->GetComponentType<Transform>());
		Coordinator->SetSystemSignature<MeshRenderer>(sig);
		//Light
		Coordinator->RegisterComponent<Light>();
		Coordinator->RegisterComponent<DirectionalLight>();
		Coordinator->RegisterSystem<LightRenderer>(LightRenderer(Renderer, SceneLights));
		Signature LightSig;
		//LightSig.set(Coordinator->GetComponentType<Light>());
		LightSig.set(Coordinator->GetComponentType<DirectionalLight>());
		Coordinator->SetSystemSignature<LightRenderer>(LightSig);
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
		// TODO refactor camera to have component on entity so this system can function as intended
		Coordinator->RegisterSystem<RSCameraPass>(RSCameraPass(Renderer));
		Coordinator->RegisterSystem<RSRenderPass>(RSRenderPass(Renderer, ShaderIDMap));
		Signature RSRenderPassSig;
		RSRenderPassSig.set(Coordinator->GetComponentType<Mesh>());
		RSRenderPassSig.set(Coordinator->GetComponentType<Material>());
		Coordinator->SetSystemSignature<RSRenderPass>(RSRenderPassSig);

	}

	ModelInfo Engine::ProcessModel(const char* Path)
	{
		Model Tmp = Model();
		return Tmp.LoadMesh(Path);
	}

	void Engine::ProcessInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		const float CameraSpeed = 5.0f * DeltaTime; // adjust accordingly
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			Renderer->RendererCamera.CameraPos += CameraSpeed * Renderer->RendererCamera.CameraFront;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			Renderer->RendererCamera.CameraPos -= CameraSpeed * Renderer->RendererCamera.CameraFront;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			Renderer->RendererCamera.CameraPos -= glm::normalize(glm::cross(Renderer->RendererCamera.CameraFront, Renderer->RendererCamera.CameraUp)) * CameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			Renderer->RendererCamera.CameraPos += glm::normalize(glm::cross(Renderer->RendererCamera.CameraFront, Renderer->RendererCamera.CameraUp)) * CameraSpeed;
		}
		
	}

	Entity Engine::CreateEntity()
	{
		return Coordinator->CreateEntity();
	}


	void Engine::Run()
	{
		Coordinator->InitSystems();
		while (!glfwWindowShouldClose(window->GetWindowPtr()))
		{
			Renderer->RendererCamera.CameraFront = CameraDirection;
			float CurrentFrame = glfwGetTime();
			DeltaTime = CurrentFrame - LastFrame;
			LastFrame = CurrentFrame;

			ProcessInput(window->GetWindowPtr());

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			Coordinator->RunSystems(DeltaTime);
			Coordinator->RunRenderSystems();
			
			RazorGUI->Render(*std::move(window));

			glfwSwapBuffers(window->GetWindowPtr());
			glfwPollEvents();
		}
		glfwTerminate();
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