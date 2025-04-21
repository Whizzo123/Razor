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
		SystemMgr = std::make_unique<SystemManager>();
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
}