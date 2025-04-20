#include "ComponentManager.h"

namespace Razor
{
	void ComponentManager::EntityDestroyed(Entity InEntity)
	{
		for (auto const& Pair : ComponentArrays)
		{
			auto const& Component = Pair.second;

			Component->EntityDestroyed(InEntity);
		}
	}

	std::vector<const char*> ComponentManager::GetComponentsForEntity(Entity InEntity)
	{
		std::vector<const char*> Components;
		for (auto const& Array: ComponentArrays)
		{
			const std::shared_ptr<IComponentArray> Ptr = Array.second;
			if (Ptr->HasData(InEntity))
			{
				Components.push_back(Array.first);
			}
		}
		return Components;
	}
}