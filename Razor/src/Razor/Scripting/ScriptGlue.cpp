#include "ScriptGlue.h"
#include <Coral/HostInstance.hpp>
#include "../Log.h"
#include "../Engine.h"
#include "../Scene/Scene.h"
#include "../Component.h"
#include "../Core/Entity.h"
#include "ScriptClass.h"
#include "ScriptInterface.h"
#include "../IO/RazorIO.h"

#if defined(_MSC_VER)
    #define RAZOR_CALL __cdecl
#else
    #define RAZOR_CALL
#endif


namespace Razor
{

	static bool Entity_HasComponent()
	{
		return true;
	}
extern "C"
{
	static void RAZOR_CALL Print_Message(const char* message)
	{
		RZ_INFO(message);
	}
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

	static int Scene_GetEntitiesWithScriptComponent(int id, uint32_t* outBuffer, int* outCount)
	{
		static std::vector<uint32_t> ids;
		ids.clear();
		Ref<Scene> scene = Engine::Get().CurrentScene;
		ScriptInterface& interface = Engine::Get().GetScriptInterface();
		auto entities = scene->GetEntitiesWithComponents<ScriptComponent>();
		for (const entt::entity& entity : entities)
		{
			ScriptComponent& comp = scene->GetEntity(entity)->GetComponent<ScriptComponent>();
			for (uint64_t instanceHandle : comp.mScriptInstances)
			{
				ScriptInstance& instance = interface.GetScriptInstance(instanceHandle);
				if (interface.GetManagedTypeId(interface.GetType(instance.className)) == id)
				{
					ids.push_back(static_cast<uint32_t>(entity));
				}
			}
		}
		
		if (outBuffer == nullptr)
		{
			*outCount = (int)ids.size();
			return 0;
		}

		int writeCount = std::min(*outCount, (int)ids.size());
		memcpy(outBuffer, ids.data(), writeCount * sizeof(uint32_t));

		*outCount = (int)ids.size();
		return writeCount;
	}

	static int Util_GetTypeIdForManagedType(const char* fullName)
	{
		ScriptInterface& interface = Engine::Get().GetScriptInterface();
		return interface.GetManagedTypeId(interface.GetType(fullName));
	}

	static void* Scene_GetComponentOnEntity(int typeId, uint32_t entityId)
	{
		Ref<Scene> scene = Engine::Get().CurrentScene;
		ScriptInterface& interface = Engine::Get().GetScriptInterface();

		Ref<Entity> entity = scene->GetEntity(static_cast<entt::entity>(entityId));
		ScriptComponent& comp = entity->GetComponent<ScriptComponent>();
		for (uint64_t instanceHandle : comp.mScriptInstances)
		{
			ScriptInstance& instance = interface.GetScriptInstance(instanceHandle);
			if (interface.GetManagedTypeId(interface.GetType(instance.className)) == typeId)
			{
				return interface.GetManagedObject(instance.handle)->GetHandle();
			}
		}

		return nullptr;
	}
	
	static int RAZOR_CALL Input_GetKey(int keyCode)
	{
		if (!Engine::Get().IsGameInputEnabled())
			return 0;
		return (int)RazorIO::Get().GetStateForKey(static_cast<RazorKey>(keyCode));
	}

	static void RAZOR_CALL Transform_GetPosition(uint32_t entityId, float* x, float* y, float* z)
	{
		Ref<Entity> entity = Engine::Get().CurrentScene->GetEntity(static_cast<entt::entity>(entityId));
		if (!entity) return;
		Transform& t = entity->GetComponent<Transform>();
		*x = t.Position.x; *y = t.Position.y; *z = t.Position.z;
	}

	static void RAZOR_CALL Transform_SetPosition(uint32_t entityId, float x, float y, float z)
	{
		Ref<Entity> entity = Engine::Get().CurrentScene->GetEntity(static_cast<entt::entity>(entityId));
		if (!entity) return;
		entity->GetComponent<Transform>().Position = glm::vec3(x, y, z);
	}

	static int RAZOR_CALL Collision_GetEventCount(uint32_t entityId)
	{
		Ref<Entity> entity = Engine::Get().CurrentScene->GetEntity(static_cast<entt::entity>(entityId));
		if (!entity || !entity->HasComponent<CollisionComponent>()) return 0;
		return static_cast<int>(entity->GetComponent<CollisionComponent>().Events.size());
	}

	static void RAZOR_CALL Collision_GetEvent(uint32_t entityId, int index, int* outType, uint32_t* outOtherId)
	{
		Ref<Entity> entity = Engine::Get().CurrentScene->GetEntity(static_cast<entt::entity>(entityId));
		if (!entity || !entity->HasComponent<CollisionComponent>()) return;
		const auto& events = entity->GetComponent<CollisionComponent>().Events;
		if (index < 0 || index >= static_cast<int>(events.size())) return;
		*outType    = static_cast<int>(events[index].Type);
		*outOtherId = events[index].OtherEntityId;
	}

	void ScriptGlue::RegisterFunctions(Ref<Coral::ManagedAssembly> Assembly)
	{
		Assembly->AddInternalCall("Razor.InternalCalls", "Entity_HasComponent", (void*)Entity_HasComponent);
		Assembly->AddInternalCall("Razor.InternalCalls", "Print_Message", (void*)Print_Message);
		Assembly->AddInternalCall("Razor.InternalCalls", "Scene_GetEntitiesWithTransforms", (void*)Scene_GetEntitiesWithTransforms);
		Assembly->AddInternalCall("Razor.InternalCalls", "Scene_GetEntitiesWithScriptComponent", (void*)Scene_GetEntitiesWithScriptComponent);
		Assembly->AddInternalCall("Razor.InternalCalls", "Util_GetTypeIdForManagedType", (void*)Util_GetTypeIdForManagedType);
		Assembly->AddInternalCall("Razor.InternalCalls", "Scene_GetComponentOnEntity", (void*)Scene_GetComponentOnEntity);
		Assembly->AddInternalCall("Razor.InternalCalls", "Input_GetKey",           (void*)Input_GetKey);
		Assembly->AddInternalCall("Razor.InternalCalls", "Transform_GetPosition",    (void*)Transform_GetPosition);
		Assembly->AddInternalCall("Razor.InternalCalls", "Transform_SetPosition",    (void*)Transform_SetPosition);
		Assembly->AddInternalCall("Razor.InternalCalls", "Collision_GetEventCount", (void*)Collision_GetEventCount);
		Assembly->AddInternalCall("Razor.InternalCalls", "Collision_GetEvent",      (void*)Collision_GetEvent);

		Assembly->UploadInternalCalls();
	}

}