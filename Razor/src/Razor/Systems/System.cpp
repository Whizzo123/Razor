#include "System.h"

namespace Razor
{

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

			System->Run(dt);
		}
	}

	void SystemManager::RunRenderSystems()
	{
		//RENDER_STAGE_MATERIALS
		RenderPipeline.RunSystemsFor(RenderStage::RENDER_STAGE_MATERIAL_PASS);
		//RENDER_STAGE_LIGHTING
		//RENDER_STAGE_TRANSFORMATIONS
		//RENDER_STAGE_CAMERA
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
