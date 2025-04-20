#pragma once

#include "Core.h"
#include "Log.h"
#include <cstdint>
#include <queue>
#include <memory>
#include <array>
#include <unordered_map>
#include <bitset>
#include <cassert>

namespace Razor
{
	using Entity = std::uint32_t;
	const Entity MAX_ENTITIES = 5000;

	using ComponentType = std::uint8_t;
	const ComponentType MAX_COMPONENTS = 32;
	using Signature = std::bitset<MAX_COMPONENTS>;


	class   EntityManager
	{
	public:
		EntityManager();

		Entity CreateEntity();
		void DestroyEntity(Entity InEntity);
		void SetSignature(Entity InEntity, Signature InSignature);
		Signature GetSignature(Entity InEntity);
		uint32_t GetCurrentEntityCount();

	private:
		std::queue<Entity> AvaliableEntities;
		std::array<Signature, MAX_ENTITIES> Signatures{};
		uint32_t LivingEntityCount = 0;
	};

	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(Entity InEntity) = 0;
		virtual bool HasData(Entity InEntity) = 0;
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
			if (EntityToIndexMap.find(InEntity) != EntityToIndexMap.end())
			{
				RZ_CORE_ERROR("Trying to insert data twice for component");
			}
			size_t NewIndex = Size;
			EntityToIndexMap[InEntity] = NewIndex;
			IndexToEntityMap[NewIndex] = InEntity;
			Components[NewIndex] = Component;
			++Size;
		}

		bool HasData(Entity InEntity)
		{
			if (!EntityToIndexMap.count(InEntity))
			{
				return false;
			}
			return true;
		}

		T& GetData(Entity InEntity)
		{
			if (!EntityToIndexMap.count(InEntity))
			{
				RZ_CORE_ERROR("Retrieving non-existent component.");
			}

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
}




