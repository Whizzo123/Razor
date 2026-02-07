#pragma once
#include <entt/entt.hpp>
#include "../Core.h"
#include <vector>

namespace Razor
{
	class Entity;
	struct ScriptObject;
	struct ScriptClass;
	struct ScriptInstance;

	class RAZOR_API Scene
	{
	public:
		Scene(const std::string& Path);
		~Scene();

		Scene(const Scene&) = delete; // No copy construction
		Scene& operator=(const Scene&) = delete; // No copy assigment

		Scene(Scene&& other) noexcept { MoveFrom(std::move(other)); }
		Scene& operator=(Scene && other) noexcept 
		{
			if (this != &other)
			{
				MoveFrom(std::move(other));
			}
			return *this;
		}

		std::string& GetPath() { return FilePath; }

		Ref<Entity> CreateEntity();

		Ref<Entity> GetEntity(entt::entity EntityHandle);

		template<typename... T>
		auto GetEntitiesWithComponents()
		{
			return registry.view<T...>();
		}

		template<typename T>
		T& GetComponent(entt::entity Entity)
		{
			return registry.get<T>(Entity);
		}

		void RunSystems(float DeltaTime);
		void CreateSystemInstance(const Razor::ScriptClass& Type);
		std::vector<std::string> GetSystemNames() const;

		void StartScene();
		void CreateInstanceObjects(std::vector<uint64_t> handles);
		void PopulateObjectFields(ScriptInstance& instance, uint64_t objId);
		void StopScene();

		

		entt::registry registry;
	private:
		void MoveFrom(Scene&& other);

	private:
		std::string FilePath;
		std::vector<uint64_t> mSystemInstanceHandles;

		friend class SceneSerializer;
	};
}

