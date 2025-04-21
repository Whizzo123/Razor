#pragma once

#include "../Scene/Scene.h"

namespace Razor
{
	class Entity
	{
	public:
		Entity() {}
		Entity(entt::entity Handle, Scene* Scene) : EntityHandle(Handle), _Scene(Scene) {}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			T& component = _Scene->registry.emplace<T>(EntityHandle, std::forward<Args>(args)...);
			return component;
		}

		entt::entity EntityHandle;

	private:
		Scene* _Scene;
	};
}

