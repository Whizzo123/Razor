#include "ScriptGlue.h"
#include <Coral/HostInstance.hpp>
#include "../Log.h"

namespace Razor
{

	static bool Entity_HasComponent()
	{
		return true;
	}

	static void __cdecl Print_Message(const char* message)
	{
		RZ_INFO(message);
	}
	
	void ScriptGlue::RegisterFunctions(Ref<Coral::ManagedAssembly> Assembly)
	{
		Assembly->AddInternalCall("Razor.InternalCalls", "Entity_HasComponent", (void*)Entity_HasComponent);
		Assembly->AddInternalCall("Razor.InternalCalls", "Print_Message", (void*)Print_Message);

		Assembly->UploadInternalCalls();
	}

}