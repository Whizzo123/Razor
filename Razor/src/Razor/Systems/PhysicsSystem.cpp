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

			IPhysicsEngine& physicsEngine = Engine::Get().GetPhysicsEngine();

			Vector3 physicsPos = physicsEngine.GetPosition(body.bodyId);
			std::vector<ContactInfo> info = physicsEngine.GetContactInfo(body.bodyId);
			for (const ContactInfo& contact : info)
			{
				if (contact.mContactType == EContactType::Started)
				{
					if (body.OnCollisionStarted)
					{
						body.OnCollisionStarted();
					}
				}
			}

			// TODO this may result in some snappy movements may need to use some smoothing here
			transform.Position = { physicsPos.X, physicsPos.Y, physicsPos.Z };

			
		}
	}
}