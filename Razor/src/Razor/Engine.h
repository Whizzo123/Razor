#pragma once

#include <vector>
#include "Window.h"
#include "Coordinator.h"
#include "Core.h"
#include "Renderer/IRenderer.h"

namespace Razor
{
	class Engine
	{
	
	public:
		Engine()
		{
			Init();
		}
		
		void Run();

		Entity CreateEntity();
		template<typename T>
		void AddComponent(Entity InEntity, T Component)
		{
			Coordinator->AddComponent<T>(InEntity, Component);
		}

		void ProcessInput(GLFWwindow* window);
	public:
		std::shared_ptr<IRenderer> Renderer;
	private:
		void Init();
	private:
		std::unique_ptr<Window> window;
		std::shared_ptr<Coordinator> Coordinator;
		float DeltaTime = 0.0f;
		float LastFrame = 0.0f;
	};
}