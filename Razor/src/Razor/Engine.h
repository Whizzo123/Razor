#pragma once

#include <vector>
#include <unordered_map>
#include "Core.h"
#include "../Platform/Generic/IPlatformIO.h"
#include "Scene/Project.h"
#include <thread>
#include <atomic>

namespace Razor
{
	// Forward Declarations
	class RazorImGui;
	class Window;
	class Coordinator;
	class IRenderer;
	class Model;
	class Shader;
	class ITimeProvider;
	class Scene;
	class ScriptInterface;

	struct Light;
	struct RenderStageConfig;

	struct ScriptAssembly;

	using RenderPipelineConfig = std::vector<RenderStageConfig>;

	/**
	* Class that is responsible for loading up all the different pieces of the engine
	* 
	* Exists as a singleton instance can only be accessed through Get() function
	*/
	class RAZOR_API Engine
	{
	
	public:
		/**
		* Getter for the Renderer
		* 
		* @return Shared Ptr to IRenderer object
		*/
		std::shared_ptr<IRenderer> GetRenderer();

		/**
		* Deconstructor for the Engine object
		*/
		~Engine();
		/**
		* Function to return the singleton instance of the Engine
		* 
		* @return Reference to the Engine
		*/
		static Engine& Get();
	protected:
		/**
		* Default Engine Constructor
		*/
		Engine();
		
	public:
		/**
		* Function to calculate engine DeltaTime
		*/
		void Step();
		/**
		* Function to initialise all systems registered to the Coordinator
		*/
		void InitSystems();
		/**
		* Getter function for the DeltaTime variable
		* 
		* @return A float representing the DeltaTime
		*/
		float GetDeltaTime() { return DeltaTime; }
		/**
		* Function to determine if engine should close if the window has recieved an exit instruction
		* 
		* @return A boolean representing if we should close the engine
		*/
		bool ShouldEngineClose();
		/**
		* Function to run the systems registered to the Coordinator
		*/
		void RunSystems();
		/**
		* Function to run the render systems registered to the Coordinator 
		* 
		* @param Config - The pipeline configuration we want to run with this render
		*/
		void Render(int32_t targetId, const RenderPipelineConfig& Config);
		/**
		* Getter function for the RazorImGui object
		* 
		* This moves the RazorGUI unique_ptr ownership to whoever called this function
		* 
		* @return A reference to the RazorImGui object
		*/
		RazorImGui& GetGUI() { return *RazorGUI; }
		/**
		* Getter function for the Window object
		* 
		* This moves the EngineWindow unique_ptr ownership to whoever called this function
		* 
		* @return A reference to the Window object
		*/
		Window& GetWindow();
		/**
		* Getter function for the Coordinator object
		* 
		* @return A shared ptr to the Coordinator object
		*/
		std::shared_ptr<Coordinator> GetCoordinator();

		/**
		* Function to Process Input via the RazorIO class
		* 
		* This just checks for the close event on the window
		*/
		void ProcessInput();
		// TODO move this
		/**
		* Function to process Model from .obj file to Model object
		*
		* @param Path - File path to .obj file
		* 
		* @return A Model object
		*/
		Model ProcessModel(const char* Path);

		std::shared_ptr<IRenderer> Renderer; /** Shared Ptr to IRenderer object used for rendering */
		/**
		* Function to initialise different parts of the engine and set them up
		*/
		void Init();
		/**
		* Getter function for a Shader from ID
		*
		* @param ID - A uint8_t representing the ID of the Shader
		* 
		* @return A shared ptr to the Shader object
		*/
		std::shared_ptr<Shader> GetShaderForID(uint8_t ID);
		/**
		* Getter function for a Shader from Type
		*
		* @param Type - A std::string representing the type of the object
		* 
		* @return A shared ptr to the Shader object
		*/
		std::shared_ptr<Shader> GetShaderForType(const char* Type);

		ScriptInterface& GetScriptInterface();

		void SaveProject();
		void LoadProject(const std::string& ProjectPath);

		void RuntimeStart();
		void RuntimeStop();

		Ref<Scene> CurrentScene; /** Ref to the current scene we have*/

	private:
		
		void RenderImGui(uint64_t SceneTexture);
		void RunRuntime();

		std::unique_ptr<Window> EngineWindow;
		std::shared_ptr<Coordinator> Coordinator;
		std::unordered_map<uint8_t, std::shared_ptr<Shader>> ShaderIDMap;
		std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderTypeMap;
		std::shared_ptr<std::vector<Light*>> SceneLights;
		std::unique_ptr<RazorImGui> RazorGUI;
		float DeltaTime = 0.0f;
		float LastFrame = 0.0f;
		static Engine* GEngine;
		std::unique_ptr<IPlatformIO> PlatformIO;
		std::unique_ptr<ITimeProvider> TimeProvider; /** Generic object to provide time functionality */
		std::unique_ptr<ScriptInterface> ScriptInterface;
		Ref<Project> LoadedProject;
		Scope<ScriptAssembly> BridgeAssembly;
		Scope<ScriptAssembly> GameAssembly;

		std::atomic<bool> bIsRuntimeRunning { false };
		std::thread RuntimeThread;
	};
}