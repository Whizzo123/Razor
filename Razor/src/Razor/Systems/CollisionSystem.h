#pragma once
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{
	struct Transform;
	struct Collider;

	class CollisionSystem : public System
	{
	public:

		CollisionSystem(Ref<Razor::Scene> Scene) : System(Scene)
		{
		}
		void Run(float dt) override;
	};
}

