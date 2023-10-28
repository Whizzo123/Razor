#include "Coordinator.h"

namespace Razor
{

	std::shared_ptr<Coordinator> Coordinator::Instance = nullptr;

	void Coordinator::Init()
	{
		EntityMgr = std::make_unique<EntityManager>();
		ComponentMgr = std::make_unique<ComponentManager>();
		SystemMgr = std::make_unique<SystemManager>();
	}

	Entity Coordinator::CreateEntity()
	{
		return EntityMgr->CreateEntity();
	}

	void Coordinator::DestroyEntity(Entity InEntity)
	{
		EntityMgr->DestroyEntity(InEntity);
		ComponentMgr->EntityDestroyed(InEntity);
		SystemMgr->EntityDestroyed(InEntity);
	}

}