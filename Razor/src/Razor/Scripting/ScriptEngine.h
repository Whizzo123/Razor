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
		std::unordered_map<uint32_t, ScriptFieldMap> EntityScriptFields;
		std::vector<ScriptInstance> ScriptInstances;

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
		static std::vector<ScriptClass> GetComponentClasses();
		static Coral::Type* GetManagedType(ScriptClass type);

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
			case ScriptFieldType::Float:   return "System.Float";
			case ScriptFieldType::Double:  return "System.Double";
			case ScriptFieldType::Bool:    return "System.Bool";
			case ScriptFieldType::Char:    return "System.Char";
			case ScriptFieldType::String:    return "System.String";
			case ScriptFieldType::Byte:    return "System.Byte";
			case ScriptFieldType::Short:   return "System.Short";
			case ScriptFieldType::Int:     return "System.Int";
			case ScriptFieldType::Long:    return "System.Long";
			case ScriptFieldType::UByte:   return "System.UByte";
			case ScriptFieldType::UShort:  return "System.UShort";
			case ScriptFieldType::UInt:    return "System.UInt";
			case ScriptFieldType::ULong:   return "System.ULong";
			case ScriptFieldType::Vector2: return "Razor.Vector2";
			case ScriptFieldType::Vector3: return "Razor.Vector3";
			case ScriptFieldType::Vector4: return "Razor.Vector4";
			case ScriptFieldType::Entity:  return "Razor.Entity";
			}
			RZ_CORE_ERROR("Unknown ScriptFieldType");
			return "None";
		}

		inline ScriptFieldType ScriptFieldTypeFromString(std::string_view fieldType)
		{
			if (fieldType == "None")    return ScriptFieldType::None;
			if (fieldType == "System.Float")   return ScriptFieldType::Float;
			if (fieldType == "System.Double")  return ScriptFieldType::Double;
			if (fieldType == "System.Bool")    return ScriptFieldType::Bool;
			if (fieldType == "System.Char")    return ScriptFieldType::Char;
			if (fieldType == "System.String")  return ScriptFieldType::String;
			if (fieldType == "System.Byte")    return ScriptFieldType::Byte;
			if (fieldType == "System.Short")   return ScriptFieldType::Short;
			if (fieldType == "System.Int")     return ScriptFieldType::Int;
			if (fieldType == "System.Long")    return ScriptFieldType::Long;
			if (fieldType == "System.UByte")   return ScriptFieldType::UByte;
			if (fieldType == "System.UShort")  return ScriptFieldType::UShort;
			if (fieldType == "System.UInt")    return ScriptFieldType::UInt;
			if (fieldType == "System.ULong")   return ScriptFieldType::ULong;
			if (fieldType == "System.Vector2") return ScriptFieldType::Vector2;
			if (fieldType == "System.Vector3") return ScriptFieldType::Vector3;
			if (fieldType == "System.Vector4") return ScriptFieldType::Vector4;
			if (fieldType == "Razor.Entity")  return ScriptFieldType::Entity;

			RZ_CORE_ERROR("Unknown ScriptFieldType");
			return ScriptFieldType::None;
		}



	}
}



