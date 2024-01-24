#pragma once
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{
	class CollisionSystem : public System
	{
	public:

		CollisionSystem()
		{
			Coordinator = Razor::Coordinator::GetInstance();
		}
		void Run(float dt) override;

	private:
		std::shared_ptr<Razor::Coordinator> Coordinator;
	};
}

