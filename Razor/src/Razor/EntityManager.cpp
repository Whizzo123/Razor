#include "EntityManager.h"

namespace Razor
{

	EntityManager::EntityManager()
	{
		for (Entity e = 0; e < MAX_ENTITIES; ++e)
		{
			AvaliableEntities.push(e);
		}
	}

	Entity EntityManager::CreateEntity()
	{
		assert(LivingEntityCount < MAX_ENTITIES && "Reached max entity count");

		Entity ID = AvaliableEntities.front();
		AvaliableEntities.pop();
		++LivingEntityCount;
		return ID;
	}

	void EntityManager::DestroyEntity(Entity InEntity)
	{
		assert(InEntity < MAX_ENTITIES && "Entity ID is not in range");

		Signatures[InEntity].reset();
		AvaliableEntities.push(InEntity);
		--LivingEntityCount;
	}

	void EntityManager::SetSignature(Entity InEntity, Signature InSignature)
	{
		assert(InEntity < MAX_ENTITIES && "Entity out of range");
		Signatures[InEntity] = InSignature;
	}

	Signature EntityManager::GetSignature(Entity InEntity)
	{
		assert(InEntity < MAX_ENTITIES && "Entity out of range");
		return Signatures[InEntity];
	}

	uint32_t EntityManager::GetCurrentEntityCount()
	{
		return LivingEntityCount;
	}

}