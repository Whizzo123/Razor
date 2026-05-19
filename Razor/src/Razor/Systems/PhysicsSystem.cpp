#include "PhysicsSystem.h"
#include "../Physics/Components/BoxBody.h"
#include "../Engine.h"
#include "../Physics/IPhysicsEngine.h"
#include "../Utils/Vector.h"

namespace Razor
{
	void PhysicsSystem::Run(float deltaTime)
	{
		for (auto entity : CurrentScene->GetEntitiesWithComponents<BoxBody, Transform>())
		{
			
			BoxBody& body = CurrentScene->GetComponent<BoxBody>(entity);
			Transform& transform = CurrentScene->GetComponent<Transform>(entity);
			Vector3 physicsPos = Engine::Get().GetPhysicsEngine().GetPosition(body.bodyId);
			if(body.mMotionType != EPhysicsMotionType::Kinematic)
			{
				// TODO this may result in some snappy movements may need to use some smoothing here
				transform.Position = { physicsPos.X, physicsPos.Y, physicsPos.Z };
			}
			else
			{
				const float fixedStep = 1.0f / 60.0f;
				Engine::Get().GetPhysicsEngine().MoveKinematic(body.bodyId, {transform.Position.x, transform.Position.y, transform.Position.z}, fixedStep);
			}
		}
	}
}
