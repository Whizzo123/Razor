#include "ScriptClass.h"
#include "ScriptEngine.h"

namespace Razor
{
	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool IsSystemClass = false, bool isComponentClass = false, bool isCore = false)
		: m_ClassNamespace(classNamespace),
		m_ClassName(className),
		m_IsSystemClass(IsSystemClass),
		m_IsComponentClass(isComponentClass),
		m_IsCore(isCore)
	{

	}
}