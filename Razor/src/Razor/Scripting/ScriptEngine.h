#pragma once
#include "../Core.h"
#include <map>
#include <unordered_map>
#include <filesystem>
#include "../Log.h"
#include <Coral/HostInstance.hpp>

namespace Razor
{
	class Scene;
	struct ScriptAssembly;
	class Entity;

	enum class ScriptFieldType
	{
		None = 0,
		Float, Double,
		Bool, Char, Byte, Short, Int, Long,
		UByte, UShort, UInt, ULong,
		Vector2, Vector3, Vector4,
		Entity,
	};

	struct ScriptField
	{
		Coral::Type Type;
		Coral::String Name;
		Coral::FieldInfo ClassField;

		ScriptFieldType GetType() const
		{
			Coral::ManagedType managedType = Type.GetManagedType();
			switch (managedType)
			{
			case Coral::ManagedType::Float:		return ScriptFieldType::Float;
			case Coral::ManagedType::Double:	return ScriptFieldType::Double;
			case Coral::ManagedType::Bool:		return ScriptFieldType::Bool;
			case Coral::ManagedType::Byte:		return ScriptFieldType::Char;
			case Coral::ManagedType::UInt:		return ScriptFieldType::UInt;
			case Coral::ManagedType::Int:		return ScriptFieldType::Int;
			default:
				return ScriptFieldType::None;
			}
		}
	};

	// ScriptField + data storage
	struct ScriptFieldInstance
	{
		ScriptField Field;

		ScriptFieldInstance()
		{
			memset(m_Buffer, 0, sizeof(m_Buffer));
		}

		template<typename T>
		T GetValue()
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			return *(T*)m_Buffer;
		}

		template<typename T>
		void SetValue(T value)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			memcpy(m_Buffer, &value, sizeof(T));
		}
	private:
		uint8_t m_Buffer[16];
	};

	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;


	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className, bool IsSystemClass = false, bool isCore = false);

		const std::map<std::string, ScriptField>& GetFields() const { return m_Fields; }

		bool IsSystemClass() const { return m_IsSystemClass; }

		const std::string& GetName() const { return m_ClassName; }

		operator bool() const { return !m_ClassName.empty(); }
	private:
		bool m_IsSystemClass = false;
		std::string m_ClassNamespace;
		std::string m_ClassName;

		std::map<std::string, ScriptField> m_Fields;

		friend class ScriptEngine;
	};

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



