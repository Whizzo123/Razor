#include "ScriptEngine.h"
#include "ScriptInterface.h"
#include "../Core/Entity.h"

#if defined(RZ_PLATFORM_WINDOWS)
#include <windows.h>
#endif

namespace Razor
{
	Coral::HostInstance ScriptEngine::CoralInstance = Coral::HostInstance();
	Coral::AssemblyLoadContext ScriptEngine::Context = Coral::AssemblyLoadContext();
	bool ScriptEngine::ScriptEngineInitialised = false;
	Scope<ScriptEngineData> ScriptEngine::s_Data = CreateScope<ScriptEngineData>();

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
		Coral::HostSettings HostSettings;

#if defined(RZ_PLATFORM_WINDOWS)
		char exePath[MAX_PATH];
		GetModuleFileNameA(NULL, exePath, MAX_PATH);
		std::string coralDir = std::filesystem::path(exePath).parent_path().string();
#elif defined(RZ_PLATFORM_LINUX)
		std::string coralDir = std::filesystem::canonical("/proc/self/exe").parent_path().string();
#endif
		HostSettings.CoralDirectory = coralDir;
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

	Coral::ManagedAssembly& ScriptEngine::LoadAssembly(const std::string& AssemblyPath)
	{
		if (!ScriptEngineInitialised)
		{
			RZ_CORE_ERROR("ScriptEngine: -> Attempting to load assembly before scriptengine is initialised");
			Coral::ManagedAssembly emptyAssembly;
			return emptyAssembly;
		}
		Coral::ManagedAssembly& assembly = Context.LoadAssembly(AssemblyPath);

		for (Coral::Type Type : assembly.GetLocalTypes())
		{
			std::string Name = Type.GetFullName();
			const char* splitter = ".";
			bool bIsSystem = false;
			bool bIsComponent = false;
			// Traverse base type chain via pointer — never assign through the reference,
			// which would overwrite TypeCache entries and corrupt m_Id for later types.
			Coral::Type* baseTypePtr = &Type.GetBaseType();
			while (*baseTypePtr)
			{
				if (baseTypePtr->GetFullName() == "Razor.System") {
					RZ_CORE_INFO("Found System Class: {0}",  std::string(Type.GetFullName()));
					bIsSystem = true;
					break;
				}
				if (baseTypePtr->GetFullName() == "Razor.Component") {
					RZ_CORE_INFO("Found Component Class: {0}", std::string(Type.GetFullName()));
					bIsComponent = true;
					break;
				}
				baseTypePtr = &baseTypePtr->GetBaseType();
			}

			Ref<ScriptClass> Class = CreateRef<ScriptClass>(std::strtok(&Name[0], splitter), Type.GetFullName(), bIsSystem, bIsComponent);
			s_Data->ScriptClasses[Type.GetFullName()] = Class;

			for (Coral::FieldInfo& Field : Type.GetFields())
			{
				Class->m_Fields[Field.GetName()] = { Field.GetType().GetFullName(), Field.GetName()};
			}
		}

		return assembly;
	}

	void ScriptEngine::Shutdown()
	{
		CoralInstance.UnloadAssemblyLoadContext(Context);
		CoralInstance.Shutdown();
	}

	Ref<ScriptClass> ScriptEngine::GetEntityClass(const std::string& name)
	{
		if (s_Data->ScriptClasses.find(name) == s_Data->ScriptClasses.end())
			return nullptr;

		return s_Data->ScriptClasses.at(name);
	}

	ScriptFieldMap& ScriptEngine::GetScriptFieldMap(Entity entity)
	{
		return s_Data->EntityScriptFields[static_cast<uint32_t>(entity.EntityHandle)];
	}

	std::vector<ScriptClass> ScriptEngine::GetSystemClasses()
	{
		std::vector<ScriptClass> systemClasses;
		for (const auto& [name, scriptClass] : s_Data->ScriptClasses)
		{
			if (scriptClass->IsSystemClass())
			{
				systemClasses.push_back(*scriptClass);
			}
		}
		return systemClasses;
	}

	std::vector<ScriptClass> ScriptEngine::GetComponentClasses()
	{
		std::vector<ScriptClass> componentClasses;
		for (const auto& [name, scriptClass] : s_Data->ScriptClasses)
		{
			if (scriptClass->m_IsComponentClass)
			{
				componentClasses.push_back(*scriptClass);
			}
		}
		return componentClasses;
	}
}