#pragma once

#include "../Scene/Scene.h"

namespace Razor
{
	class RAZOR_API Entity
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

		void AddScriptComponent(std::string className);

		template<typename T>
		bool HasComponent()
		{
			return _Scene->registry.try_get<T>(EntityHandle) != nullptr;
		}

		template<typename T>
		T& GetComponent()
		{
			return _Scene->registry.get<T>(EntityHandle);
		}

		template<typename T>
		bool RemoveComponent()
		{
			if(_Scene->registry.try_get<T>(EntityHandle))
			{
				_Scene->registry.remove<T>(EntityHandle);
				return true;
			}
			return false;
		}

		entt::entity EntityHandle;

	private:
		Scene* _Scene;
	};
}

