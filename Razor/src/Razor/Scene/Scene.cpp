#include "Scene.h"
#include "../Core/Entity.h"

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
		return CreateRef<Entity>(EntityHandle, this);
	}
	
}