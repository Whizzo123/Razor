#include "ScriptInterface.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"
#include "../Log.h"
#include <Coral/HostInstance.hpp>
#include <filesystem>

namespace Razor
{
	ScriptInterface::ScriptInterface()
	{

	}

	ScriptInterface::~ScriptInterface() = default;

	ScriptInterface::ScriptInterface(ScriptInterface&&) noexcept = default;

	ScriptInterface& ScriptInterface::operator=(ScriptInterface&&) noexcept = default;

	Scope<ScriptAssembly> ScriptInterface::LoadAssembly(std::string binaryPath, std::string assemblyName, bool isBridgeAssembly)
	{
		std::string absolutePath = std::filesystem::absolute(binaryPath).string();

		std::optional<std::string> assemblyPath = SearchFiles(binaryPath, assemblyName);

		Scope<Coral::ManagedAssembly> assembly = ScriptEngine::LoadAssembly(assemblyPath.value());
		if(!assembly)
		{
			return nullptr;
		}

		if (assembly->GetLoadStatus() != Coral::AssemblyLoadStatus::Success)
		{
			RZ_CORE_ERROR("ScriptInterface: -> Failed to load assembly at path: {0}", absolutePath);
			return nullptr;
		}

		if (isBridgeAssembly)
		{
			ScriptGlue::RegisterFunctions(*assembly);
		}
		AssemblyPool.emplace_back(std::move(assembly));
		Scope<ScriptAssembly> scriptAssembly = CreateScope<ScriptAssembly>();
		scriptAssembly->assemblyIndex = static_cast<int>(AssemblyPool.size()) - 1;
		return scriptAssembly;
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
			Coral::Type& objType = assembly->GetLocalType(type.GetName());
			if (objType)
			{
				return GetType(objType.GetBaseType().GetFullName());
			}
		}
		return ScriptClass();
	}

	ScriptObject ScriptInterface::CreateInstance(ScriptClass type)
	{
		for (auto& assembly : AssemblyPool)
		{
			Coral::Type& objType = assembly->GetLocalType(type.GetName());
			if (objType)
			{
				ObjectPool.push_back(Razor::CreateRef<Coral::ManagedObject>(objType.CreateInstance()));
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
			Coral::Type& objType = assembly->GetLocalType(type.GetName());
			if (objType)
			{
				ScriptInstance inst = ScriptInstance{ 0, type.GetName(), {}};
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

	void ScriptInterface::InvokeMethod(int handle, const std::string& methodName, float param)
	{
		ObjectPool[handle]->InvokeMethod(methodName, param);
	}

	std::vector<ScriptClass> ScriptInterface::GetSystemTypes()
	{
		return ScriptEngine::GetSystemClasses();
	}

	std::vector<ScriptClass> ScriptInterface::GetComponentTypes()
	{
		return ScriptEngine::GetComponentClasses();
	}

	// TODO if this can be null should be ptr
	ScriptInstance& ScriptInterface::GetScriptInstance(uint64_t instanceId)
	{
		if (instanceId >= ScriptInstancePool.size())
		{
			RZ_CORE_ERROR("ScriptInterface(GetScriptInstance): -> Invalid instance ID: {0}", instanceId);
			throw std::out_of_range("Invalid ScriptInstance ID");
		}
		return ScriptInstancePool[instanceId];
	}

	int ScriptInterface::GetManagedTypeId(ScriptClass type)
	{
		for (auto& assembly : AssemblyPool)
		{
			Coral::Type& objType = assembly->GetLocalType(type.GetName());
			if (objType)
			{
				return objType.GetTypeId();
			}
		}
		return -1;
	}
	
	Ref<Coral::ManagedObject> ScriptInterface::GetManagedObject(int handle)
	{
		if (handle < 0 || handle >= static_cast<int>(ObjectPool.size()))
		{
			RZ_CORE_ERROR("ScriptInterface(GetManagedObject): -> Invalid object handle: {0}", handle);
			return nullptr;
		}
		return ObjectPool[handle];
	}

	void ScriptInterface::DestroyInstanceObject(int handle)
	{
		ScriptInstance& instance = GetScriptInstance(handle);
		Ref<Coral::ManagedObject> object = GetManagedObject(instance.handle);
		if (object)
			object->Destroy();
		instance.handle = 0;
	}

	void ScriptInterface::ClearObjectPool()
	{
		ObjectPool.clear();
	}

	std::optional<std::string> ScriptInterface::SearchFiles(const std::string& path, const std::string& searchFileName)
	{
		try
		{
			for (const std::filesystem::directory_entry& DirectoryEntry : std::filesystem::directory_iterator(path))
			{
				const std::filesystem::path& filePath = DirectoryEntry.path();
				// Do some substring logic on path to get filename
				const std::string fileName = filePath.filename().string();
				if(fileName == searchFileName)
				{
					return std::optional<std::string>(filePath.string());
				}

				if (DirectoryEntry.is_directory())
				{
					std::optional<std::string> file = SearchFiles(filePath.string(), searchFileName);
					if (file.has_value())
					{
						return file;
					}
				}
			}
		}
		catch (const std::filesystem::filesystem_error& e)
		{
			RZ_CORE_ERROR("ScriptInterface::SearchFiles: Failed to iterate directory '{0}': {1}", path, e.what());
		}

		return std::nullopt;
	}
}