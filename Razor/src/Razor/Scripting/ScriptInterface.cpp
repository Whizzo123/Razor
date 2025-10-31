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
		Razor::Ref<Coral::ManagedAssembly> Assembly = ScriptEngine::LoadAssembly(assemblyPath);
		if (Assembly == nullptr)
		{
			RZ_CORE_ERROR("ScriptInterface: -> Failed to load assembly at path: {0}", assemblyPath);
			return ScriptAssembly { -1 };
		}
		if (isBridgeAssembly)
		{
			ScriptGlue::RegisterFunctions(Assembly);
		}
		AssemblyPool.push_back(Assembly);
		return ScriptAssembly { static_cast<int>(AssemblyPool.size()) - 1};
	}

	ScriptType ScriptInterface::GetType(ScriptAssembly assembly, const std::string& typeName)
	{
		if (typeName == "System.Object")
		{
			return ScriptType();
		}
		Razor::Ref<Coral::ManagedAssembly> searchingAssembly = AssemblyPool[assembly.assemblyIndex];

		Coral::Type& returnedType = searchingAssembly->GetType(typeName);

		if (returnedType)
		{
			int32_t id = returnedType.GetTypeId();
			if (TypePool.find(id) != TypePool.end())
			{
				return TypePool[id];
			}
			else
			{
				int baseId = -1;
				if (returnedType.GetBaseType())
				{
					ScriptType baseType = GetType(assembly, returnedType.GetBaseType().GetFullName());
					baseId = baseType.id;
				}
				ScriptType type{ id, returnedType.GetFullName(), baseId, assembly};
				TypePool[id] = type;
				return type;
			}
		}
		else
		{
			return ScriptType();
		}
		
	}

	ScriptType ScriptInterface::GetBaseType(ScriptType type)
	{
		if (type.baseId < 0)
		{
			return ScriptType();
		}

		if (TypePool.find(type.baseId) != TypePool.end())
		{
			return TypePool[type.baseId];
		}
		else
		{
			RZ_CORE_ERROR("Managed to get a ScriptType with no base type but a valid base id");
			return ScriptType();
		}
		
	}

	std::vector<ScriptType> ScriptInterface::GetTypes(ScriptAssembly assembly)
	{
		std::vector<Coral::Type*> types = AssemblyPool[assembly.assemblyIndex]->GetTypes();
		std::vector<ScriptType> scriptTypes;
		for (Coral::Type* entry : types)
		{
			scriptTypes.push_back(GetType(assembly, entry->GetFullName()));
		}
		return scriptTypes;
	}

	ScriptObject ScriptInterface::CreateInstance(ScriptType type)
	{
		Razor::Ref<Coral::ManagedAssembly> instanceAssembly = AssemblyPool[type.assembly.assemblyIndex];
		Coral::Type& instanceType = instanceAssembly->GetType(type.fullName);
		ObjectPool.push_back(std::move(Razor::CreateRef<Coral::ManagedObject>(instanceType.CreateInstance())));
		return ScriptObject {static_cast<int>(ObjectPool.size()) - 1, type};
	}
	void ScriptInterface::InvokeMethod(ScriptObject object, const std::string& methodName, float param)
	{
		ObjectPool[object.id]->InvokeMethod(methodName, param);
	}
}