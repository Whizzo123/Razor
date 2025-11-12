#pragma once
#include "../Core.h"
#include <map>
#include <unordered_map>
#include <filesystem>
#include "../Log.h"
#include <Coral/HostInstance.hpp>
#include "ScriptClass.h"

namespace Razor
{
	class Scene;
	struct ScriptAssembly;
	class Entity;

	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

	struct ScriptEngineData
	{
		Scope<ScriptAssembly> BridgeAssembly = nullptr;
		Scope<ScriptAssembly> GameAssembly = nullptr;

		std::filesystem::path CoreAssemblyFilepath;
		std::filesystem::path AppAssemblyFilepath;

		ScriptClass EntityClass;

		std::unordered_map<std::string, Ref<ScriptClass>> ScriptClasses;
		//std::unordered_map<UUID, Ref<ScriptInstance>> EntityInstances;
		std::unordered_map<uint32_t, ScriptFieldMap> EntityScriptFields;

		//Scope<filewatch::FileWatch<std::string>> AppAssemblyFileWatcher;
		//bool AssemblyReloadPending = false;

		// Runtime

		Scene* SceneContext = nullptr;
	};

	class RAZOR_API ScriptEngine
	{

	public:
		static void Init();
		static void Shutdown();
		static Coral::ManagedAssembly& LoadAssembly(const std::string& AssemblyPath);
		static Ref<ScriptClass> GetEntityClass(const std::string& name);
		static ScriptFieldMap& GetScriptFieldMap(Entity entity);
		static std::vector<ScriptClass> GetSystemClasses();

	private:
		static Coral::HostInstance CoralInstance;
		static Coral::AssemblyLoadContext Context;
		static bool ScriptEngineInitialised;
		static Scope<ScriptEngineData> s_Data;
	};


	namespace Utils {

		inline const char* ScriptFieldTypeToString(ScriptFieldType fieldType)
		{
			switch (fieldType)
			{
			case ScriptFieldType::None:    return "None";
			case ScriptFieldType::Float:   return "Float";
			case ScriptFieldType::Double:  return "Double";
			case ScriptFieldType::Bool:    return "Bool";
			case ScriptFieldType::Char:    return "Char";
			case ScriptFieldType::Byte:    return "Byte";
			case ScriptFieldType::Short:   return "Short";
			case ScriptFieldType::Int:     return "Int";
			case ScriptFieldType::Long:    return "Long";
			case ScriptFieldType::UByte:   return "UByte";
			case ScriptFieldType::UShort:  return "UShort";
			case ScriptFieldType::UInt:    return "UInt";
			case ScriptFieldType::ULong:   return "ULong";
			case ScriptFieldType::Vector2: return "Vector2";
			case ScriptFieldType::Vector3: return "Vector3";
			case ScriptFieldType::Vector4: return "Vector4";
			case ScriptFieldType::Entity:  return "Entity";
			}
			RZ_CORE_ERROR("Unknown ScriptFieldType");
			return "None";
		}

		inline ScriptFieldType ScriptFieldTypeFromString(std::string_view fieldType)
		{
			if (fieldType == "None")    return ScriptFieldType::None;
			if (fieldType == "Float")   return ScriptFieldType::Float;
			if (fieldType == "Double")  return ScriptFieldType::Double;
			if (fieldType == "Bool")    return ScriptFieldType::Bool;
			if (fieldType == "Char")    return ScriptFieldType::Char;
			if (fieldType == "Byte")    return ScriptFieldType::Byte;
			if (fieldType == "Short")   return ScriptFieldType::Short;
			if (fieldType == "Int")     return ScriptFieldType::Int;
			if (fieldType == "Long")    return ScriptFieldType::Long;
			if (fieldType == "UByte")   return ScriptFieldType::UByte;
			if (fieldType == "UShort")  return ScriptFieldType::UShort;
			if (fieldType == "UInt")    return ScriptFieldType::UInt;
			if (fieldType == "ULong")   return ScriptFieldType::ULong;
			if (fieldType == "Vector2") return ScriptFieldType::Vector2;
			if (fieldType == "Vector3") return ScriptFieldType::Vector3;
			if (fieldType == "Vector4") return ScriptFieldType::Vector4;
			if (fieldType == "Entity")  return ScriptFieldType::Entity;

			RZ_CORE_ERROR("Unknown ScriptFieldType");
			return ScriptFieldType::None;
		}



	}
}



