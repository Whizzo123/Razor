#pragma once
#include <Coral/HostInstance.hpp>


namespace Razor
{
	class ScriptEngine
	{

	public:
		static void Init();
		static void Shutdown();
		static void LoadAssembly(const std::string& AssemblyPath);

	private:
		static Coral::HostInstance CoralInstance;
		static Coral::AssemblyLoadContext Context;
		static bool ScriptEngineInitialised;
	};
}

