#include "Coordinator.h"

namespace Razor
{
	std::shared_ptr<Coordinator> Coordinator::Instance = nullptr;

	std::shared_ptr<Coordinator> Coordinator::GetInstance()
	{
		if (!Instance)
		{
			Instance = std::make_shared<Coordinator>();
		}
		return Instance;
	};


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

	uint32_t Coordinator::GetCurrentEntityCount()
	{
		return EntityMgr->GetCurrentEntityCount();
	}

	void Coordinator::RunSystems(float dt)
	{
		SystemMgr->RunSystems(dt);
	}

	void Coordinator::RunRenderSystems(RenderPipelineConfig PipelineConfig)
	{
		SystemMgr->RunRenderSystems(PipelineConfig);
	}

	void Coordinator::InitSystems()
	{
		SystemMgr->InitSystems();
	}

	std::vector<const char*> Coordinator::GetComponentsForEntity(Entity InEntity)
	{
		return ComponentMgr->GetComponentsForEntity(InEntity);
	}
}