#pragma once
#include <cstdint>
#include <queue>
#include <memory>
#include <array>
#include <unordered_map>
#include <bitset>
#include <cassert>

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;


class EntityManager
{
public:
	EntityManager();

	Entity CreateEntity();
	void DestroyEntity(Entity InEntity);
	void SetSignature(Entity InEntity, Signature InSignature);
	Signature GetSignature(Entity InEntity);

private:
	std::queue<Entity> AvaliableEntities;
	std::array<Signature, MAX_ENTITIES> Signatures{};
	uint32_t LivingEntityCount;
};

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity InEntity) = 0;
};


template<typename T>
class ComponentArray : public  IComponentArray
{
public:
	void RemoveData(Entity InEntity)
	{
		assert(EntityToIndexMap.find(InEntity) != EntityToIndexMap.end() && "Removing non-existent component.");

		// Copy element at end into deleted element's place to maintain density
		size_t IndexOfRemovedEntity = EntityToIndexMap[InEntity];
		size_t IndexOfLastElement = Size - 1;
		Components[IndexOfRemovedEntity] = Components[IndexOfLastElement];

		// Update map to point to moved spot
		Entity EntityOfLastElement = IndexToEntityMap[IndexOfLastElement];
		EntityToIndexMap[EntityOfLastElement] = IndexOfRemovedEntity;
		IndexToEntityMap[IndexOfRemovedEntity] = EntityOfLastElement;

		EntityToIndexMap.erase(InEntity);
		IndexToEntityMap.erase(IndexOfLastElement);

		--Size;
	}
	
	void InsertData(Entity InEntity, T Component)
	{
		assert(EntityToIndexMap.find(InEntity) == EntityToIndexMap.end() && "Trying to insert data twice");
		size_t NewIndex = Size;
		EntityToIndexMap[InEntity] = NewIndex;
		IndexToEntityMap[NewIndex] = InEntity;
		Components[NewIndex] = Component;
		++Size;
	}

	T& GetData(Entity InEntity)
	{
		assert(EntityToIndexMap.find(InEntity) != EntityToIndexMap.end() && "Retrieving non-existent component.");

		// Return a reference to the entity's component
		return Components[EntityToIndexMap[InEntity]];
	}
	
	void EntityDestroyed(Entity InEntity)
	{
		if (EntityToIndexMap.find(InEntity) != EntityToIndexMap.end())
		{
			// Remove the entity's component if it existed
			RemoveData(InEntity);
		}
	}

private:
	std::array<T, MAX_ENTITIES> Components;
	std::unordered_map<Entity, size_t> EntityToIndexMap;
	std::unordered_map<size_t, Entity> IndexToEntityMap;
	size_t Size;
};




