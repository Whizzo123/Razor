#pragma once

#include <string>
#include <map>
#include <vector>
#include "../Log.h"

namespace Razor
{
	enum class RAZOR_API ScriptFieldType
	{
		None = 0,
		Float, Double,
		Bool, Char, Byte, Short, Int, Long,
		UByte, UShort, UInt, ULong,
		Vector2, Vector3, Vector4,
		Entity, String,
	};

	struct RAZOR_API ScriptField
	{
		//Figure out how ScriptClass and ScriptField fits together
		std::string Type;
		std::string Name;
		//Coral::FieldInfo ClassField;

		ScriptFieldType GetType() const;
	};

	// ScriptField + data storage
	struct RAZOR_API ScriptFieldInstance
	{
		ScriptField Field;

		ScriptFieldInstance()
		{
			memset(m_Buffer, 0, sizeof(m_Buffer));
		}

		template<typename T>
		T GetValue()
		{
			static_assert(sizeof(T) <= 64, "Type too large!");
			return *(T*)m_Buffer;
		}

		template<typename T>
		void SetValue(T value)
		{
			static_assert(sizeof(T) <= 64, "Type too large!");
			memcpy(m_Buffer, &value, sizeof(T));
		}
	private:
		uint8_t m_Buffer[64];
	};


	struct RAZOR_API ScriptInstance {
		uint64_t handle;
		std::string className;

		std::vector<ScriptFieldInstance> fields;

		ScriptFieldInstance& GetFieldInstance(const std::string& name) {
			for (auto& fieldInstance : fields) {
				if (fieldInstance.Field.Name == name) {
					return fieldInstance;
				}
			}
			RZ_CORE_ERROR("Field not found: {0}", name);
			ScriptFieldInstance dummy;
			return dummy;
		}
	};

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className, bool IsSystemClass = false, bool isComponentClass = false, bool isCore = false);

		const std::map<std::string, ScriptField>& GetFields() const { return m_Fields; }

		bool IsSystemClass() const { return m_IsSystemClass; }

		const std::string& GetName() const { return m_ClassName; }

		operator bool() const { return !m_ClassName.empty(); }
	private:
		bool m_IsSystemClass = false;
		bool m_IsComponentClass = false;
		bool m_IsCore = false;
		std::string m_ClassNamespace;
		std::string m_ClassName;

		std::map<std::string, ScriptField> m_Fields;

		friend class ScriptEngine;
	};
}

