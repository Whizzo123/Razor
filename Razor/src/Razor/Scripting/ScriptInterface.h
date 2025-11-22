#pragma once
#include "../Core.h"
#include "ScriptClass.h"
#include <vector>
#include <unordered_map>

namespace Coral
{
	class ManagedAssembly;
	class ManagedObject;
}

namespace Razor
{

	struct ScriptAssembly
	{
		int assemblyIndex = -1;

		operator bool() const
		{
			return assemblyIndex > -1;
		}
	};

	struct ScriptType
	{
		int id = -1;
		std::string fullName;
		int baseId = -1;
		ScriptAssembly assembly{ -1 };
		operator bool() const
		{
			return id;
		}
	};

	struct ScriptObject
	{
		int id;
		ScriptClass type;
	};

	class RAZOR_API ScriptInterface
	{
	public:
		ScriptInterface();
		ScriptAssembly LoadAssembly(std::string assemblyPath, bool isBridgeAssembly);
		ScriptClass GetType(const std::string& typeName);
		ScriptClass GetBaseType(ScriptClass type);
		std::vector<ScriptClass> GetSystemTypes();
		std::vector<ScriptClass> GetComponentTypes();
		ScriptObject CreateInstance(ScriptClass type);
		uint64_t CreateScriptInstance(ScriptClass type);
		void InvokeMethod(ScriptObject object, const std::string& methodName, float param);
		ScriptInstance& GetScriptInstance(uint64_t instanceId);
		int GetManagedTypeId(ScriptClass type);
		Ref<Coral::ManagedObject> GetManagedObject(int handle);

	private:
		std::vector<Razor::Ref<Coral::ManagedObject>> ObjectPool;
		std::unordered_map<int32_t, ScriptType> TypePool;
		std::vector<Ref<Coral::ManagedAssembly>> AssemblyPool;
		std::vector<ScriptInstance> ScriptInstancePool;
	};
}

