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
#include "../Renderer/Font/Text.h"

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
		auto entities = Engine::Get().mCurrentScene->GetEntitiesWithComponents<Transform>();
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
		Ref<Scene> scene = Engine::Get().mCurrentScene;
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
		Ref<Scene> scene = Engine::Get().mCurrentScene;
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
		Ref<Entity> entity = Engine::Get().mCurrentScene->GetEntity(static_cast<entt::entity>(entityId));
		if (!entity) return;
		Transform& t = entity->GetComponent<Transform>();
		*x = t.Position.x; *y = t.Position.y; *z = t.Position.z;
	}

	static void RAZOR_CALL Transform_SetPosition(uint32_t entityId, float x, float y, float z)
	{
		Ref<Entity> entity = Engine::Get().mCurrentScene->GetEntity(static_cast<entt::entity>(entityId));
		if (!entity) return;
		entity->GetComponent<Transform>().Position = glm::vec3(x, y, z);
	}

	static int RAZOR_CALL Collision_GetEventCount(uint32_t entityId)
	{
		Ref<Entity> entity = Engine::Get().mCurrentScene->GetEntity(static_cast<entt::entity>(entityId));
		if (!entity || !entity->HasComponent<CollisionComponent>()) return 0;
		return static_cast<int>(entity->GetComponent<CollisionComponent>().Events.size());
	}

	static void RAZOR_CALL Collision_GetEvent(uint32_t entityId, int index, int* outType, uint32_t* outOtherId, float** outHitX, float** outHitY, float** outHitZ, int* hitLen)
	{
		Ref<Entity> entity = Engine::Get().mCurrentScene->GetEntity(static_cast<entt::entity>(entityId));
		if (!entity || !entity->HasComponent<CollisionComponent>()) return;
		const auto& events = entity->GetComponent<CollisionComponent>().Events;
		if (index < 0 || index >= static_cast<int>(events.size())) return;
		*outType    = static_cast<int>(events[index].Type);
		*outOtherId = events[index].OtherEntityId;
		static std::vector<float> x;
		static std::vector<float> y;
		static std::vector<float> z;

		*hitLen = events[index].mCollisionPoints.size();

		for(int i = 0; i < *hitLen; i++)
		{
			Vector3 pos = events[index].mCollisionPoints[i];
			x.push_back(pos.X);
			y.push_back(pos.Y);
			z.push_back(pos.Z);
		}
		*outHitX = &x[0];
		*outHitY = &y[0];
		*outHitZ = &z[0];
	}

	 static void RAZOR_CALL Text_SetText(uint32_t entityId, const char* text)
 	{
 	    Ref<Entity> entity = Engine::Get().mCurrentScene->GetEntity(static_cast<entt::entity>(entityId));
 	    if (!entity || !entity->HasComponent<Text>()) return;
 	    entity->GetComponent<Text>().SetText(text);
 	}

 	static void RAZOR_CALL Text_GetText(uint32_t entityId, char* buffer, int bufferSize)
 	{
 	    Ref<Entity> entity = Engine::Get().mCurrentScene->GetEntity(static_cast<entt::entity>(entityId));
 	    if (!entity || !entity->HasComponent<Text>()) return;
 	    const std::string& txt = entity->GetComponent<Text>().GetText();
 	    strncpy(buffer, txt.c_str(), bufferSize - 1);
 	    buffer[bufferSize - 1] = '\0';
 	}

 	static void RAZOR_CALL Text_SetColor(uint32_t entityId, float r, float g, float b)
 	{
 	    Ref<Entity> entity = Engine::Get().mCurrentScene->GetEntity(static_cast<entt::entity>(entityId));
 	    if (!entity || !entity->HasComponent<Text>()) return;
 	    entity->GetComponent<Text>().mColor = Vector3(r, g, b);
 	}

 	static void RAZOR_CALL Text_SetScale(uint32_t entityId, float scale)
 	{
 	    Ref<Entity> entity = Engine::Get().mCurrentScene->GetEntity(static_cast<entt::entity>(entityId));
 	    if (!entity || !entity->HasComponent<Text>()) return;
 	    entity->GetComponent<Text>().mScale = scale;
 	}

 	static void* Scene_GetEntitiesWithText(int* count)
 	{
 	    static std::vector<uint32_t> ids;
 	    ids.clear();
 	    auto entities = Engine::Get().mCurrentScene->GetEntitiesWithComponents<Text>();
 	    for (const entt::entity& e : entities)
 	        ids.push_back(static_cast<uint32_t>(e));
 	    *count = static_cast<int>(ids.size());
 	    return ids.data();
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
		Assembly->AddInternalCall("Razor.InternalCalls", "Text_SetText",              (void*)Text_SetText);
 		Assembly->AddInternalCall("Razor.InternalCalls", "Text_GetText",              (void*)Text_GetText);
 		Assembly->AddInternalCall("Razor.InternalCalls", "Text_SetColor",             (void*)Text_SetColor);
 		Assembly->AddInternalCall("Razor.InternalCalls", "Text_SetScale",             (void*)Text_SetScale);
 		Assembly->AddInternalCall("Razor.InternalCalls", "Scene_GetEntitiesWithText", (void*)Scene_GetEntitiesWithText);

		Assembly->UploadInternalCalls();
	}

}