#include "ScriptGlue.h"
#include <Coral/HostInstance.hpp>
#include "../Log.h"
#include "../Engine.h"
#include "../Scene/Scene.h"
#include "../Component.h"

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

	static void* Scene_GetEntitiesWithTransforms(int* count)
	{
		static std::vector<uint32_t> ids;
		ids.clear();
		auto entities = Engine::Get().CurrentScene->GetEntitiesWithComponents<Transform>();
		for (const entt::entity& entity : entities) 
		{
			ids.push_back(static_cast<uint32_t>(entity));
		}
		*count = static_cast<int>(ids.size());
		return ids.data();
	}
	
	void ScriptGlue::RegisterFunctions(Ref<Coral::ManagedAssembly> Assembly)
	{
		Assembly->AddInternalCall("Razor.InternalCalls", "Entity_HasComponent", (void*)Entity_HasComponent);
		Assembly->AddInternalCall("Razor.InternalCalls", "Print_Message", (void*)Print_Message);
		Assembly->AddInternalCall("Razor.InternalCalls", "Scene_GetEntitiesWithTransforms", (void*)Scene_GetEntitiesWithTransforms);

		Assembly->UploadInternalCalls();
	}

}