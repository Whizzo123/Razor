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

		CollisionSystem()
		{
			Coordinator = Razor::Coordinator::GetInstance();
			Signature = { Coordinator->GetComponentType<Transform>(), Coordinator->GetComponentType<Collider>() };
		}
		void Run(float dt) override;

	private:
		std::shared_ptr<Razor::Coordinator> Coordinator;
	};
}

