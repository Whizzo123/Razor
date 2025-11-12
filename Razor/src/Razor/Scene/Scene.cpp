#include "Scene.h"
#include "../Core/Entity.h"
#include "../Component.h"
#include <string>
#include "../Engine.h"
#include "../Scripting/ScriptInterface.h"
#include "../Scripting/ScriptEngine.h"
#include "../Log.h"

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
		ent->AddComponent<Transform>();
		return ent;
	}
	
	Ref<Entity> Scene::GetEntity(entt::entity EntityHandle)
	{
		Ref<Entity> Entt = CreateRef<Entity>(EntityHandle, this);
		
		return Entt->HasComponent<Transform>() ? Entt : nullptr;
	}
	
	void Scene::RunSystems(float DeltaTime)
	{
		for (const auto& system : SystemObjects)
		{
			Engine::Get().GetScriptInterface().InvokeMethod(system, "Run", DeltaTime);
		}
	}

	void Scene::CreateSystemObject(const Razor::ScriptClass& Type)
	{
		RZ_CORE_INFO("Instantiating system type {0}", Type.GetName());
		SystemObjects.push_back(Engine::Get().GetScriptInterface().CreateInstance(Type));
	}

	std::vector<std::string> Scene::GetSystemNames() const
	{
		std::vector<std::string> Names(SystemObjects.size());
		for (const auto& obj : SystemObjects)
		{
			Names.push_back(obj.type.GetName());
		}
		return Names;
	}
}