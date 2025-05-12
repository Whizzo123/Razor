#include "Scene.h"
#include "../Core/Entity.h"
#include "../Component.h"

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
	
}