#include "CollisionSystem.h"
#include "../Component.h"

namespace Razor
{
	void CollisionSystem::Run(float DeltaTime)
	{
		for (Entity OurEntity : Entities)
		{
			Transform& EntityTransform = Coordinator->GetComponent<Transform>(OurEntity);
			Collider& EntityCollider = Coordinator->GetComponent<Collider>(OurEntity);
			// Compare this entity with every other entity
			for (Entity ComparisonEntity : Entities)
			{
				Transform& ComparisonTransform = Coordinator->GetComponent<Transform>(ComparisonEntity);
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