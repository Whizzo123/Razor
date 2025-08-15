#pragma once
#include "../Core.h"

namespace Coral
{
	class ManagedAssembly;
	class HostInstance;
	class AssemblyLoadContext;
}

namespace Razor
{
	class RAZOR_API ScriptEngine
	{

	public:
		static void Init();
		static void Shutdown();
		static Razor::Ref<Coral::ManagedAssembly> LoadAssembly(const std::string& AssemblyPath);

	private:
		static Coral::HostInstance CoralInstance;
		static Coral::AssemblyLoadContext Context;
		static bool ScriptEngineInitialised;
	};
}

