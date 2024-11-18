#pragma once
#include "Core.h"
#include "Log.h"
#include "EntityManager.h"


namespace Razor
{
	class   ComponentManager
	{
	public:

		ComponentManager() {};

		template<typename T>
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();
			assert(ComponentTypes.find(typeName) == ComponentTypes.end() && "Registering component type more than once");
			ComponentTypes.insert({ typeName, NextComponentType });
			ComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });
			++NextComponentType;
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			const char* typeName = typeid(T).name();
			assert(ComponentTypes.find(typeName) != ComponentTypes.end() && "Component does not exist");
			return ComponentTypes[typeName];
		}

		template<typename T>
		void AddComponent(Entity InEntity, T InComponent)
		{
			GetComponentArray<T>()->InsertData(InEntity, InComponent);
		}

		template<typename T>
		void RemoveComponent(Entity InEntity)
		{
			GetComponentArray<T>()->RemoveData(InEntity);
		}

		template<typename T>
		T& GetComponent(Entity InEntity)
		{
			return GetComponentArray<T>()->GetData(InEntity);
		}

		template<typename T>
		bool HasComponent(Entity InEntity)
		{
			return GetComponentArray<T>()->HasData(InEntity);
		}

		void EntityDestroyed(Entity InEntity);

	private:
		// Map from type string pointer to a component type
		std::unordered_map<const char*, ComponentType> ComponentTypes{};

		// Map from type string pointer to a component array
		std::unordered_map<const char*, std::shared_ptr<IComponentArray>> ComponentArrays{};

		// The component type to be assigned to the next registered component - starting at 0
		ComponentType NextComponentType{};

		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			const char* TypeName = typeid(T).name();

			if (ComponentTypes.find(TypeName) == ComponentTypes.end())
			{
				RZ_CORE_ERROR("Component %s not registered before use", TypeName);
			}

			std::shared_ptr <IComponentArray> IArray = ComponentArrays[TypeName];
			std::shared_ptr<ComponentArray<T>> Array = std::dynamic_pointer_cast<ComponentArray<T>>(IArray);
			if (Array)
			{
				return Array;
			}
			RZ_CORE_ERROR("Could not find an array for the component being set on this entity");
			return nullptr;
		}
	};

}