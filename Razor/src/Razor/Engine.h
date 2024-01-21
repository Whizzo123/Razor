#pragma once

#include <vector>
#include "Window.h"
#include "Coordinator.h"
#include "Core.h"
#include "Renderer/IRenderer.h"
#include "Renderer/Model.h"
#include "Renderer/Shaders/Shader.h"
#include "ImGui/RazorImGui.h"
// TODO note to self remove all of this static ness in favour of better ideally unique_ptr system pls :) 
namespace Razor
{
	class RAZOR_API Engine
	{
	
	public:
		std::shared_ptr<IRenderer> GetRenderer()
		{
			return Renderer;
		}
		~Engine()
		{
			RZ_CORE_INFO("Destroying razor");
			delete GEngine;
		}
		static Engine& Get()
		{
			if (GEngine == nullptr)
			{
				GEngine = new Engine();
			}
			return *GEngine;
		}
	protected:
		Engine()
		{
			
		}
		
	public:
		void Run();

		std::shared_ptr<Coordinator> GetCoordinator()
		{
			return Coordinator;
		}

		Entity CreateEntity();

		template<typename T>
		void AddComponentToEntity(Entity InEntity, T Component)
		{
			AddComponent<T>(InEntity, Component);
		}
		void ProcessInput(GLFWwindow* window);
		// TODO move this
		ModelInfo ProcessModel(const char* Path);
		std::shared_ptr<IRenderer> Renderer;
		void Init();
		std::shared_ptr<Shader> GetShaderForID(uint8_t ID);
		std::shared_ptr<Shader> GetShaderForType(const char* Type);
	private:
		template<typename T>
		void AddComponent(Entity InEntity, T Component)
		{
			Coordinator->AddComponent<T>(InEntity, Component);
		}
		std::unique_ptr<Window> window;
		std::shared_ptr<Coordinator> Coordinator;
		std::unordered_map<uint8_t, std::shared_ptr<Shader>> ShaderIDMap;
		std::unordered_map<const char*, std::shared_ptr<Shader>> ShaderTypeMap;
		std::shared_ptr<std::vector<Light*>> SceneLights;
		std::unique_ptr<RazorImGui> RazorGUI;
		float DeltaTime = 0.0f;
		float LastFrame = 0.0f;
		static Engine* GEngine;
		
	};
}