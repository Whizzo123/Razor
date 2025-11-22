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
		return ScriptObject{ -1, ScriptClass() };
	}

	uint64_t ScriptInterface::CreateScriptInstance(ScriptClass type)
	{
		for (auto& assembly : AssemblyPool)
		{
			Coral::Type& objType = assembly->GetType(type.GetName());
			if (objType)
			{
				ScriptObject obj = CreateInstance(type);
				if (obj.id == -1)
					break;
				ScriptInstance inst = ScriptInstance{ static_cast<uint64_t>(obj.id), type.GetName()};
				for (const auto& [fieldName, field] : type.GetFields())
				{
					ScriptFieldInstance fieldInstance;
					fieldInstance.Field = field;
					inst.fields.push_back(fieldInstance);
				}
				ScriptInstancePool.push_back(inst);
				return ScriptInstancePool.size() - 1;
			}
		}
		return -1;
	}

	void ScriptInterface::InvokeMethod(ScriptObject object, const std::string& methodName, float param)
	{
		ObjectPool[object.id]->InvokeMethod(methodName, param);
	}

	std::vector<ScriptClass> ScriptInterface::GetSystemTypes()
	{
		return ScriptEngine::GetSystemClasses();
	}

	std::vector<ScriptClass> ScriptInterface::GetComponentTypes()
	{
		return ScriptEngine::GetComponentClasses();
	}

	ScriptInstance& ScriptInterface::GetScriptInstance(uint64_t instanceId)
	{
		if (instanceId >= ScriptInstancePool.size())
		{
			RZ_CORE_ERROR("ScriptInterface(GetScriptInstance): -> Invalid instance ID: {0}", instanceId);
			throw std::out_of_range("Invalid ScriptInstance ID");
		}
		return ScriptInstancePool[instanceId];
	}
}