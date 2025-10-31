#pragma once
#include "../Core.h"
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
		ScriptType type;
	};

	class RAZOR_API ScriptInterface
	{
	public:
		ScriptInterface();
		ScriptAssembly LoadAssembly(std::string assemblyPath, bool isBridgeAssembly);
		ScriptType GetType(ScriptAssembly assembly, const std::string& typeName);
		ScriptType GetBaseType(ScriptType type);
		std::vector<ScriptType> GetTypes(ScriptAssembly assembly);
		ScriptObject CreateInstance(ScriptType type);
		void InvokeMethod(ScriptObject object, const std::string& methodName, float param);

	private:
		std::vector<Razor::Ref<Coral::ManagedObject>> ObjectPool;
		std::unordered_map<int32_t, ScriptType> TypePool;
		std::vector<Razor::Ref<Coral::ManagedAssembly>> AssemblyPool;
	};
}

