#include "ScriptGlue.h"
#include <Coral/HostInstance.hpp>
#include "../Log.h"

namespace Razor
{

	static bool Entity_HasComponent()
	{
		return true;
	}

	static void Print_Message(std::string message)
	{
		RZ_INFO(message);
	}

	void ScriptGlue::RegisterFunctions(Coral::ManagedAssembly& Assembly)
	{
		Assembly.AddInternalCall("Razor.InternalCalls::", "#Entity_HasComponent", &Entity_HasComponent);
		Assembly.AddInternalCall("Razor.InternalCalls::", "#Print_Message", &Print_Message);

		Assembly.UploadInternalCalls();
	}

}