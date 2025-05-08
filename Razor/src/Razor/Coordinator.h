#pragma once
#include "Systems/System.h"
#include <memory>
#include "Core.h"
#include "entt/entt.hpp"
#include "Component.h"

namespace Razor
{

	class Coordinator
	{
	public:
		static std::shared_ptr<Coordinator> GetInstance();

		Coordinator() 
		{ 
			Init(); 
		};
	
		template<typename T>
		std::shared_ptr<T> RegisterSystem(T System)
		{
			return SystemMgr->RegisterSystem<T>(System);
		}

		void RunSystems(float dt);
		void RunRenderSystems(RenderPipelineConfig PipelineConfig);
		void InitSystems();
		std::vector<const char*> GetComponentsForEntity(Entity InEntity);

	private:
		void Init();

	private:
		std::unique_ptr<SystemManager> SystemMgr;
		static std::shared_ptr<Coordinator> Instance;
	};
}

