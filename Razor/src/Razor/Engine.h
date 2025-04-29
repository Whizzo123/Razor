#pragma once

#include <vector>
#include "Window.h"
#include "Coordinator.h"
#include "Core.h"
#include "Renderer/IRenderer.h"
#include "Renderer/Model.h"
#include "Renderer/Shaders/Shader.h"
#include "ImGui/RazorImGui.h"
#include "../Platform/Generic/IPlatformIO.h"
#include "imgui.h"
#include "Scene/Scene.h"
#include "entt/entt.hpp"

namespace Razor
{
	using Vec2 = ImVec2;

	class   Engine
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
		void Step();
		void InitSystems();
		float GetDeltaTime() { return DeltaTime; }
		bool ShouldEngineClose() { return EngineWindow->ShouldWindowClose(); }
		void RunSystems() { Coordinator->RunSystems(DeltaTime); }
		void RunRenderSystems(const RenderPipelineConfig& Config) { Coordinator->RunRenderSystems(Config); }
		RazorImGui& GetGUI() { return std::move(*RazorGUI); }
		Window& GetWindow() { return std::move(*EngineWindow); }
		std::shared_ptr<Coordinator> GetCoordinator()
		{
			return Coordinator;
		}

		void ProcessInput();
		void PickObject(unsigned int PickBuffer);
		// TODO move this
		Model ProcessModel(const char* Path);
		std::shared_ptr<IRenderer> Renderer;
		void Init();
		std::shared_ptr<Shader> GetShaderForID(uint8_t ID);
		std::shared_ptr<Shader> GetShaderForType(const char* Type);
		Ref<Scene> CurrentScene;

	private:
		
		void RenderImGui(uint64_t SceneTexture);

		std::unique_ptr<Window> EngineWindow;
		std::shared_ptr<Coordinator> Coordinator;
		std::unordered_map<uint8_t, std::shared_ptr<Shader>> ShaderIDMap;
		std::unordered_map<const char*, std::shared_ptr<Shader>> ShaderTypeMap;
		std::shared_ptr<std::vector<Light*>> SceneLights;
		std::unique_ptr<RazorImGui> RazorGUI;
		float DeltaTime = 0.0f;
		float LastFrame = 0.0f;
		static Engine* GEngine;
		std::unique_ptr<IPlatformIO> PlatformIO;
		
	};
}