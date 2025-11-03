#include "Scene.h"
#include "../Core/Entity.h"
#include "../Component.h"
#include <string>
#include "../Engine.h"
#include "../Scripting/ScriptInterface.h"

namespace Razor
{
	Scene::Scene(const std::string& Path)
	{
		FilePath = Path;
	}
	Scene::~Scene()
	{

	}

	Ref<Entity> Scene::CreateEntity()
	{
		auto entityHandle = registry.create();
		Ref<Entity> ent = CreateRef<Entity>(entityHandle, this);
		return ent;
	}
	
	Ref<Entity> Scene::GetEntity(entt::entity EntityHandle)
	{
		Ref<Entity> Entt = CreateRef<Entity>(EntityHandle, this);
		
		return Entt->HasComponent<Transform>() ? Entt : nullptr;
	}
	
	void Scene::RunSystems(float DeltaTime)
	{
		for (auto system : SystemObjects)
		{
			Engine::Get().GetScriptInterface().InvokeMethod(system, "Run", DeltaTime);
		}
	}

	void Scene::CreateSystemObject()
	{
		/*for (Razor::ScriptType ScriptType : Types)
		{
			RZ_CORE_INFO("Script Type {0} and name {1}", ScriptType.id, ScriptType.fullName);
			RZ_CORE_INFO("Script Type Base Type {0} and name {1}", ScriptInterface->GetBaseType(ScriptType).id, ScriptInterface->GetBaseType(ScriptType).fullName);
			RZ_CORE_INFO("Razor System Type Id {0}", ScriptInterface->GetType(*(BridgeAssembly.get()), "Razor.System").id);
			if (ScriptInterface->GetBaseType(ScriptType).id == ScriptInterface->GetType(*(BridgeAssembly.get()), "Razor.System").id)
			{
				RZ_INFO("Instantiating system type");
				Razor::ScriptObject TestObject = ScriptInterface->CreateInstance(ScriptType);

				// 2 real questions here is how do we hold all these system objects and how do we give them the entities to work with
			}
		}*/
	}
}