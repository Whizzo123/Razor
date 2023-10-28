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
}