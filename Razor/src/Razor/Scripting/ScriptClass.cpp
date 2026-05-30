#include "ScriptClass.h"
#include "ScriptEngine.h"

namespace Razor
{
	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool IsSystemClass, bool isComponentClass, bool isCore)
		: m_ClassNamespace(classNamespace),
		m_ClassName(className),
		m_IsSystemClass(IsSystemClass),
		m_IsComponentClass(isComponentClass),
		m_IsCore(isCore)
	{

	}

	ScriptFieldType ScriptField::GetType() const
	{
		if (Type == "None")    return ScriptFieldType::None;
		if (Type == "System.Single")   return ScriptFieldType::Float;
		if (Type == "System.Double")  return ScriptFieldType::Double;
		if (Type == "System.Bool")    return ScriptFieldType::Bool;
		if (Type == "System.Char")    return ScriptFieldType::Char;
		if (Type == "System.String")  return ScriptFieldType::String;
		if (Type == "System.Byte")    return ScriptFieldType::Byte;
		if (Type == "System.Short")   return ScriptFieldType::Short;
		if (Type == "System.Int32")   return ScriptFieldType::Int;
		if (Type == "System.Long")    return ScriptFieldType::Long;
		if (Type == "System.UByte")   return ScriptFieldType::UByte;
		if (Type == "System.UShort")  return ScriptFieldType::UShort;
		if (Type == "System.UInt")    return ScriptFieldType::UInt;
		if (Type == "System.ULong")   return ScriptFieldType::ULong;
		if (Type == "System.Vector2") return ScriptFieldType::Vector2;
		if (Type == "System.Vector3") return ScriptFieldType::Vector3;
		if (Type == "System.Vector4") return ScriptFieldType::Vector4;
		if (Type == "Razor.Entity")  return ScriptFieldType::Entity;

		RZ_CORE_ERROR("ScriptField::GetType() -> Unknown ScriptFieldType {0}", Type);
		return ScriptFieldType::None;
	}
}