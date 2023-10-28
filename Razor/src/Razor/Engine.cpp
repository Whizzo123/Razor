#include "Engine.h"
#include "MeshRenderer.h"
#include "Component.h"
#include "Renderer/OpenGLRenderer.h"

namespace Razor
{

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
		Renderer->RendererCamera.CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		Renderer->RendererCamera.CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		Renderer->RendererCamera.CameraDirection = glm::normalize(Renderer->RendererCamera.CameraPos - Renderer->RendererCamera.CameraTarget);

		Renderer->RendererCamera.Up = glm::vec3(0.0f, 1.0f, 0.0f);
		Renderer->RendererCamera.CameraRight = glm::normalize(glm::cross(Renderer->RendererCamera.Up, Renderer->RendererCamera.CameraDirection));
		Renderer->RendererCamera.CameraUp = glm::cross(Renderer->RendererCamera.CameraDirection, Renderer->RendererCamera.CameraRight);
		Renderer->RendererCamera.CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
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
			float CurrentFrame = glfwGetTime();
			DeltaTime = CurrentFrame - LastFrame;
			LastFrame = CurrentFrame;

			ProcessInput(window->GetWindowPtr());

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			Coordinator->RunSystems(DeltaTime);

			glfwSwapBuffers(window->GetWindowPtr());
			glfwPollEvents();
		}

		glfwTerminate();
	}
}