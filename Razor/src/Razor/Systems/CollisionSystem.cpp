#include "CollisionSystem.h"
#include "../Component.h"
#include "../Physics/Components/BoxBody.h"
#include "../Physics/IPhysicsEngine.h"
#include "../Engine.h"
#include <unordered_map>

namespace Razor
{
	void CollisionSystem::Run([[maybe_unused]]float dt)
	{
		// Built each frame — acceptable for current entity counts; cache if this becomes a hotspot
		std::unordered_map<unsigned int, uint32_t> bodyToEntity;
		for (auto entity : _mCurrentScene->GetEntitiesWithComponents<BoxBody>())
		{
			BoxBody& body = _mCurrentScene->GetComponent<BoxBody>(entity);
			// TODO what happens if an entity is destroyed?
			bodyToEntity[body.bodyId] = static_cast<uint32_t>(entity);
		}

		IPhysicsEngine& physics = Engine::Get().GetPhysicsEngine();

		for (auto entity : _mCurrentScene->GetEntitiesWithComponents<BoxBody>())
		{
			BoxBody& body = _mCurrentScene->GetComponent<BoxBody>(entity);
			CollisionComponent* collComp = _mCurrentScene->TryGetComponent<CollisionComponent>(entity);

			if (collComp)
				collComp->Events.clear();

			std::vector<ContactInfo> contacts = physics.GetContactInfo(body.bodyId);
			for (const ContactInfo& contact : contacts)
			{
				if (contact.mContactType == EContactType::Started && !contact.mContactProcessed)
				{
					if (body.OnCollisionStarted)
						body.OnCollisionStarted();

					if (collComp)
					{
						uint32_t otherId = UINT32_MAX;
						auto it = bodyToEntity.find(contact.mOtherBodyId);
						if (it != bodyToEntity.end()) otherId = it->second;
						collComp->Events.push_back({ CollisionEventType::Started, otherId, contact.mCollisionPoints });
					}
				}
				else if (contact.mContactType == EContactType::Ended && collComp)
				{
					uint32_t otherId = UINT32_MAX;
					auto it = bodyToEntity.find(contact.mOtherBodyId);
					if (it != bodyToEntity.end()) otherId = it->second;
					collComp->Events.push_back({ CollisionEventType::Ended, otherId, contact.mCollisionPoints });
				}
			}
		}
	}
}
