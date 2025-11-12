#include "ScriptInterface.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"
#include "../Log.h"
#include <Coral/HostInstance.hpp>

namespace Razor
{
	ScriptInterface::ScriptInterface()
	{

	}

	ScriptAssembly ScriptInterface::LoadAssembly(std::string assemblyPath, bool isBridgeAssembly)
	{
		AssemblyPool.push_back(CreateRef<Coral::ManagedAssembly>(ScriptEngine::LoadAssembly(assemblyPath)));

		Ref<Coral::ManagedAssembly> Assembly = AssemblyPool.back();
		if (Assembly->GetLoadStatus() == Coral::AssemblyLoadStatus::UnknownError)
		{
			RZ_CORE_ERROR("ScriptInterface: -> Failed to load assembly at path: {0}", assemblyPath);
			return ScriptAssembly{ -1 };
		}

		if (isBridgeAssembly)
		{
			ScriptGlue::RegisterFunctions(Assembly);
		}
		// This is a nightmare how do we fix it hahaha
		;
		return ScriptAssembly { static_cast<int>(AssemblyPool.size()) - 1};
	}

	ScriptClass ScriptInterface::GetType(const std::string& typeName)
	{
		Ref<ScriptClass> type = ScriptEngine::GetEntityClass(typeName);
		return type ? *type : ScriptClass();
	}

	ScriptClass ScriptInterface::GetBaseType(ScriptClass type)
	{
		for (auto& assembly : AssemblyPool)
		{
			Coral::Type& objType = assembly->GetType(type.GetName());
			if (objType)
			{
				return GetType(objType.GetBaseType().GetFullName());
			}
		}
	}

	ScriptObject ScriptInterface::CreateInstance(ScriptClass type)
	{
		for (auto& assembly : AssemblyPool)
		{
			Coral::Type& objType = assembly->GetType(type.GetName());
			if (objType)
			{
				ObjectPool.push_back(std::move(Razor::CreateRef<Coral::ManagedObject>(objType.CreateInstance())));
				return ScriptObject{ static_cast<int>(ObjectPool.size()) - 1, type };
			}
		}
		RZ_CORE_ERROR("ScriptInterface(CreateInstance): -> Failed to create instance of type: {0}", type.GetName());
	}

	void ScriptInterface::InvokeMethod(ScriptObject object, const std::string& methodName, float param)
	{
		ObjectPool[object.id]->InvokeMethod(methodName, param);
	}

	std::vector<ScriptClass> ScriptInterface::GetSystemTypes()
	{
		return ScriptEngine::GetSystemClasses();
	}
}