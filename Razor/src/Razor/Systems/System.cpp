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

	void SystemManager::RunRenderSystems()
	{
		//RENDER_STAGE_MATERIALS
		RenderPipeline.RunSystemsFor(RenderStage::RENDER_STAGE_MATERIAL_PASS);
		//RENDER_STAGE_LIGHTING
		RenderPipeline.RunSystemsFor(RenderStage::RENDER_STAGE_LIGHTING_PASS);
		//RENDER_STAGE_TRANSFORMATIONS
		RenderPipeline.RunSystemsFor(RenderStage::RENDER_STAGE_TRANSFORMATION_PASS);
		//RENDER_STAGE_CAMERA
		RenderPipeline.RunSystemsFor(RenderStage::RENDER_STAGE_CAMERA_PASS);
		// Final render from IRenderer piecing together properties with shader
		RenderPipeline.RunSystemsFor(RenderStage::RENDER_STAGE_RENDER);
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
