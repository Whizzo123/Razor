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
	class AssetDirectory;
	class IPhysicsEngine;
	class IPhysicsDebugRenderer;
	class PhysicsDebugDrawBuffer;
	class RSCameraPass;

	struct Light;
	struct RenderStageConfig;

	struct ScriptAssembly;

	using RenderPipelineConfig = std::vector<RenderStageConfig>;

	class FontLoader;

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
		Ref<IRenderer> GetRenderer();

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
		Engine(const Razor::Engine&) = delete;
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
		float GetDeltaTime() { return _mDeltaTime; }
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
		* @param config - The pipeline configuration we want to run with this render
		*/
		void Render(int32_t targetId, const RenderPipelineConfig& config);
		/**
		* Getter function for the RazorImGui object
		* 
		* This moves the _mRazorGUI unique_ptr ownership to whoever called this function
		* 
		* @return A reference to the RazorImGui object
		*/
		RazorImGui& GetGUI() { return *_mRazorGUI; }
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
		Ref<Coordinator> GetCoordinator();

		/**
		* Function to Process Input via the RazorIO class
		* 
		* This just checks for the close event on the window
		*/
		void ProcessInput();

		Ref<IRenderer> mRenderer; /** Shared Ptr to IRenderer object used for rendering */
		/**
		* Function to initialise different parts of the engine and set them up
		*/
		void Init();
		/**
		* Getter function for a Shader from ID
		*
		* @param id - A uint8_t representing the ID of the Shader
		* 
		* @return A shared ptr to the Shader object
		*/
		Ref<Shader> GetShaderForID(uint8_t id);
		/**
		* Getter function for a Shader from Type
		*
		* @param type - A std::string representing the type of the object
		* 
		* @return A shared ptr to the Shader object
		*/
		Ref<Shader> GetShaderForType(const char* type);

		ScriptInterface& GetScriptInterface();

		void SaveProject();
		void LoadProject(const std::string& projectPath);

		void RuntimeStart();
		void RuntimeStop();
		bool IsRuntimeRunning() const { return _mbIsRuntimeRunning.load(); }

		void SetGameInputEnabled(bool bEnabled) { _mbIsGameInputEnabled.store(bEnabled); }
		bool IsGameInputEnabled() const { return _mbIsGameInputEnabled.load(); }

		void SetGameCameraViewportSize(uint32_t w, uint32_t h);

		Ref<Scene> mCurrentScene; /** Ref to the current scene we have*/

		Ref<AssetDirectory> GetAssetDirectory();

		IPhysicsEngine& GetPhysicsEngine();

		FontLoader& GetFontLoader();

		void PopulateRenderPipelineDebugData();

		void ClearDebugDrawBuffer();

		void CreateProject();

	private:
		void RunRuntime();

		Scope<Window> _mEngineWindow;
		Ref<Coordinator> _mCoordinator;
		std::unordered_map<uint8_t, std::shared_ptr<Shader>> _mShaderIDMap;
		std::unordered_map<std::string, std::shared_ptr<Shader>> _mShaderTypeMap;
		Ref<std::vector<Light*>> _mSceneLights;
		Scope<RazorImGui> _mRazorGUI;
		float _mDeltaTime = 0.0f;
		float _mLastFrame = 0.0f;
		static Scope<Engine> _mGEngine;
		Scope<IPlatformIO> _mPlatformIO;
		Scope<ITimeProvider> _mTimeProvider; /** Generic object to provide time functionality */
		Scope<ScriptInterface> _mScriptInterface;
		Ref<Project> _mLoadedProject;
		Scope<ScriptAssembly> _mBridgeAssembly;
		Scope<ScriptAssembly> _mGameAssembly;
		Ref<AssetDirectory> _mAssetDirectory;
		Scope<IPhysicsEngine> _mPhysicsEngine;
		Ref<IPhysicsDebugRenderer> _mPhysicsDebugRenderer;

		Scope<PhysicsDebugDrawBuffer> _mDebugDrawBuffer;

		std::atomic<bool> _mbIsRuntimeRunning { false };
		std::atomic<bool> _mbIsGameInputEnabled { true };
		std::thread _mRuntimeThread;
		Ref<RSCameraPass> _mCameraPass;
		Scope<FontLoader> _mFontLoader;
	};
}