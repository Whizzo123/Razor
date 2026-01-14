#pragma once
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{
	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem(Ref<Razor::Scene> Scene) : System(Scene)
		{
		}
		void Run(float dt) override;
	};
}

