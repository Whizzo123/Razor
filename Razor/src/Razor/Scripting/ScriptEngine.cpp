#include "ScriptEngine.h"
#include <Coral/HostInstance.hpp>
#include "../Log.h"

namespace Razor
{
	Coral::HostInstance ScriptEngine::CoralInstance = Coral::HostInstance();
	Coral::AssemblyLoadContext ScriptEngine::Context = Coral::AssemblyLoadContext();
	bool ScriptEngine::ScriptEngineInitialised = false;

	void CoralMessageCallback(std::string_view message, Coral::MessageLevel level)
	{
		RZ_CORE_INFO("Coral Message {0}: {1}", (int)level, message);
	}

	void ExceptionCallback(std::string_view message)
	{
		RZ_CORE_ERROR("Coral Exception: {0}", message);
	}

	void ScriptEngine::Init()
	{
		std::wstring ws(std::filesystem::current_path().c_str());
		std::string s(ws.begin(), ws.end());
		RZ_CORE_INFO(s);
		Coral::HostSettings HostSettings;
		HostSettings.CoralDirectory = "bin";
		HostSettings.MessageCallback = CoralMessageCallback;
		HostSettings.ExceptionCallback = ExceptionCallback;

		if (CoralInstance.Initialize(HostSettings) != Coral::CoralInitStatus::Success)
		{
			RZ_CORE_ERROR("Failed to initialise Coral");
			return;
		}
		Context = CoralInstance.CreateAssemblyLoadContext("Game");
		RZ_CORE_INFO("ScriptEngine: -> Successfully initialised Coral");
		ScriptEngineInitialised = true;
	}

	Razor::Ref<Coral::ManagedAssembly> ScriptEngine::LoadAssembly(const std::string& AssemblyPath)
	{
		if (!ScriptEngineInitialised)
		{
			RZ_CORE_ERROR("ScriptEngine: -> Attempting to load assembly before scriptengine is initialised");
			return nullptr;
		}
		return Razor::Ref<Coral::ManagedAssembly>(&Context.LoadAssembly(AssemblyPath));
		//Coral::ManagedAssembly loadedAssembly = Context.LoadAssembly(AssemblyPath);
		/*if (loadedAssembly.GetLoadStatus() != Coral::AssemblyLoadStatus::Success)
		{
			RZ_CORE_ERROR("ScriptEngine: -> Failed to load assembly at path: {0}", AssemblyPath);
			return nullptr;
		}
		else
		{
			return Razor::Ref<Coral::ManagedAssembly>(&loadedAssembly);
		}*/
	}

	void ScriptEngine::Shutdown()
	{
		CoralInstance.UnloadAssemblyLoadContext(Context);
		CoralInstance.Shutdown();
	}
}