#include "System.h"
#include <glm/glm.hpp>

namespace Razor
{

	RenderStage RenderSystem::SystemRenderStage = RenderStage::RENDER_STAGE_MATERIAL_PASS;

	// Bare in mind that RunSystems and RunRenderSystems run on two different threads

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
				_mRenderPipeline.mPipelineSystems[StageConfig.Stage][std::string(SystemType)]->Render(_mRenderPipeline.mPipelineData);
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

	void SystemManager::SetPipelineDebugData(std::vector<DebugLine> lineData, std::vector<DebugTriangle> triangleData)
	{			
		_mRenderPipeline.mPipelineData.mDebugLineProperties.clear();
		_mRenderPipeline.mPipelineData.mDebugTriangleProperties.clear();
		_mRenderPipeline.mPipelineData.mDebugLines = lineData;
		_mRenderPipeline.mPipelineData.mDebugTriangles = triangleData;
	}

}
