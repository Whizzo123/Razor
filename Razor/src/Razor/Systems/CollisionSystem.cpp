#include "CollisionSystem.h"
#include "../Component.h"

namespace Razor
{
	void CollisionSystem::Run(float DeltaTime)
	{
		auto View = CurrentScene->GetEntitiesWithComponents<Transform, Collider>();
		for (auto OurEntity : View)
		{
			Transform& EntityTransform = CurrentScene->GetComponent<Transform>(OurEntity);
			Collider& EntityCollider = CurrentScene->GetComponent<Collider>(OurEntity);
			// Compare this entity with every other entity
			for (auto ComparisonEntity : View)
			{
				Transform& ComparisonTransform = CurrentScene->GetComponent<Transform>(ComparisonEntity);
				if (OurEntity != ComparisonEntity)
				{
					float Distance = glm::distance(EntityTransform.Position, ComparisonTransform.Position);
					if (Distance < EntityCollider.Radius)
					{
						// COLLISION
						RZ_CORE_INFO("COLLISION ALERT");
					}
				}
			}
		}
	}
}