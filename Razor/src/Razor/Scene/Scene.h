#pragma once
#include <string>
#include <entt/entt.hpp>
#include "../Core.h"

namespace Razor
{
	class Entity;

	class Scene
	{
	public:
		Scene(const std::string& Path);
		~Scene();
		std::string& GetPath() { return FilePath; }

		Ref<Entity> CreateEntity();

		Ref<Entity> GetEntity(entt::entity EntityHandle);

		template<typename... T>
		auto GetEntitiesWithComponents()
		{
			return registry.view<T...>();
		}

		template<typename T>
		T GetComponent(entt::entity Entity)
		{
			return registry.get<T>(Entity);
		}

		entt::registry registry;
	private:
		std::string FilePath;
	};
}

