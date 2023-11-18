#include "Engine.h"
#include "MeshRenderer.h"
#include "Component.h"
#include "Renderer/OpenGLRenderer.h"
#include "Renderer/Model.h"
#include "Systems/LightRenderer.h"
#include "Renderer/Shaders/DefaultMeshShader.h"
#include "Renderer/Shaders/DebugLightShader.h"

namespace Razor
{

	Engine* Engine::GEngine = nullptr;
	std::shared_ptr<Coordinator> Engine::Coordinator = nullptr;
	std::shared_ptr<IRenderer> Engine::Renderer = nullptr;

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
		GEngine = new Engine();
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		GEngine->window = std::make_unique<Window>(800, 600);
		glEnable(GL_DEPTH_TEST);
		Coordinator = Coordinator::GetInstance();
		GEngine->Renderer = std::make_shared<OpenGLRenderer>();
		//glfwSetInputMode(GEngine->window->GetWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(GEngine->window->GetWindowPtr(), Mouse_Callback);

		//CREATE SHADERS
		std::shared_ptr<Shader> D_MeshShader = std::make_shared<DefaultMeshShader>();
		GEngine->ShaderIDMap[D_MeshShader->ID] = D_MeshShader;
		GEngine->ShaderTypeMap[typeid(DefaultMeshShader).name()] = D_MeshShader;
		std::shared_ptr<Shader> D_DebugShader = std::make_shared<DebugLightShader>();
		GEngine->ShaderIDMap[D_DebugShader->ID] = D_DebugShader;
		GEngine->ShaderTypeMap[typeid(DebugLightShader).name()] = D_DebugShader;

		//Seperate this into a camera component and camera system
		GEngine->Renderer->RendererCamera.CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		GEngine->Renderer->RendererCamera.CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		GEngine->Renderer->RendererCamera.CameraDirection = glm::normalize(GEngine->Renderer->RendererCamera.CameraPos - GEngine->Renderer->RendererCamera.CameraTarget);

		GEngine->Renderer->RendererCamera.Up = glm::vec3(0.0f, 1.0f, 0.0f);
		GEngine->Renderer->RendererCamera.CameraRight = glm::normalize(glm::cross(GEngine->Renderer->RendererCamera.Up, GEngine->Renderer->RendererCamera.CameraDirection));
		GEngine->Renderer->RendererCamera.CameraUp = glm::cross(GEngine->Renderer->RendererCamera.CameraDirection, GEngine->Renderer->RendererCamera.CameraRight);
		GEngine->Renderer->RendererCamera.CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

		// Seperate into some sort of all component/system registration
		Coordinator->RegisterComponent<Mesh>();
		Coordinator->RegisterComponent<Material>();
		Coordinator->RegisterComponent<Transform>();
		GEngine->SceneLights = std::make_shared<std::vector<Light*>>();
		Coordinator->RegisterSystem<MeshRenderer>(MeshRenderer(GEngine->Renderer, GEngine->ShaderIDMap, GEngine->SceneLights));
		Signature sig;
		sig.set(Coordinator->GetComponentType<Mesh>());
		sig.set(Coordinator->GetComponentType<Material>());
		sig.set(Coordinator->GetComponentType<Transform>());
		Coordinator->SetSystemSignature<MeshRenderer>(sig);
		//Light
		Coordinator->RegisterComponent<Light>();
		Coordinator->RegisterComponent<DirectionalLight>();
		Coordinator->RegisterSystem<LightRenderer>(LightRenderer(GEngine->Renderer, GEngine->SceneLights));
		Signature LightSig;
		LightSig.set(Coordinator->GetComponentType<Light>());
		LightSig.set(Coordinator->GetComponentType<DirectionalLight>());
		Coordinator->SetSystemSignature<LightRenderer>(LightSig);

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
		return GEngine->Coordinator->CreateEntity();
	}


	void Engine::Run()
	{
		Coordinator->InitSystems();
		while (!glfwWindowShouldClose(GEngine->window->GetWindowPtr()))
		{
			GEngine->Renderer->RendererCamera.CameraFront = CameraDirection;
			float CurrentFrame = glfwGetTime();
			GEngine->DeltaTime = CurrentFrame - GEngine->LastFrame;
			GEngine->LastFrame = CurrentFrame;

			GEngine->ProcessInput(GEngine->window->GetWindowPtr());

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			Coordinator->RunSystems(GEngine->DeltaTime);

			glfwSwapBuffers(GEngine->window->GetWindowPtr());
			glfwPollEvents();
		}
		delete GEngine;
		glfwTerminate();
	}

	// This is fine to have no checks as it would return 0 anyway if there was no shader for that ID meaning we always get a shader
	std::shared_ptr<Shader> Engine::GetShaderForID(uint8_t ID)
	{
		return GEngine->ShaderIDMap[ID];
	}

	std::shared_ptr<Shader> Engine::GetShaderForType(const char* Type)
	{
		return GEngine->ShaderTypeMap[Type];
	}
}