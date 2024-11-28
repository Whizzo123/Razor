#include "System.h"

namespace Razor
{

	RenderStage RenderSystem::SystemRenderStage = RenderStage::RENDER_STAGE_MATERIAL_PASS;

	void SystemManager::EntityDestroyed(Entity InEntity)
	{
		for (auto const& Pair : Systems)
		{
			auto const& System = Pair.second;
			System->Entities.erase(InEntity);
		}
	}

	void SystemManager::EntitySignatureChanged(Entity InEntity, Signature EntitySignature)
	{
		for (auto const& Pair : Systems)
		{
			auto const& Type = Pair.first;
			auto const& System = Pair.second;
			auto const& SystemSignature = Signatures[Type];

			// Entity signature matches system signature - insert into set
			if ((EntitySignature & SystemSignature) == SystemSignature)
			{
				System->Entities.insert(InEntity);
			}
			// Entity signature does not match system signature - erase from set
			else
			{
				System->Entities.erase(InEntity);
			}
		}
	}

	void SystemManager::RunSystems(float dt)
	{
		for (auto const& Pair : Systems)
		{
			auto const& System = Pair.second;
			if (std::dynamic_pointer_cast<RenderSystem>(System))
			{
				continue;
			}
			System->Run(dt);
		}
	}

	void SystemManager::RunRenderSystems(RenderPipelineConfig& PipelineConfig)
	{
		for (const RenderStageConfig& StageConfig : PipelineConfig)
		{
			for (const char* SystemType : StageConfig.StageSystemsToRun)
			{
				RenderPipeline.PipelineSystems[StageConfig.Stage][SystemType]->Render(RenderPipeline.EntityRenderProperties);
			}
		}
	}

	void SystemManager::InitSystems()
	{
		for (auto const& Pair : Systems)
		{
			auto const& System = Pair.second;

			System->Init();
		}
	}

}
