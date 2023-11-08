#pragma once

#include <vector>
#include "Window.h"
#include "Coordinator.h"
#include "Core.h"
#include "Renderer/IRenderer.h"
#include "Renderer/Model.h"

namespace Razor
{
	class RAZOR_API Engine
	{
	
	public:
		static std::shared_ptr<IRenderer> GetRenderer()
		{
			return Renderer;
		}
		~Engine()
		{
			RZ_CORE_INFO("Destroying razor");
			
		}
	protected:
		Engine()
		{
			
		}
		
	public:
		static void Run();

		std::shared_ptr<Coordinator> GetCoordinator()
		{
			return Coordinator;
		}

		static Entity CreateEntity();

		template<typename T>
		static void AddComponentToEntity(Entity InEntity, T Component)
		{
			GEngine->AddComponent<T>(InEntity, Component);
		}
		void ProcessInput(GLFWwindow* window);
		// TODO move this
		static ModelInfo ProcessModel(const char* Path);
		static std::shared_ptr<IRenderer> Renderer;
		static void Init();
	private:
		template<typename T>
		void AddComponent(Entity InEntity, T Component)
		{
			Coordinator->AddComponent<T>(InEntity, Component);
		}
		std::unique_ptr<Window> window;
		static std::shared_ptr<Coordinator> Coordinator;
		float DeltaTime = 0.0f;
		float LastFrame = 0.0f;
		static Engine* GEngine;
	};
}