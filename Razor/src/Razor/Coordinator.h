#pragma once
#include "Systems/System.h"
#include <memory>
#include "Core.h"

namespace Razor
{

	class RAZOR_API Coordinator
	{
	public:
		static std::shared_ptr<Coordinator> GetInstance()
		{
			if (!Instance)
			{
				Instance = std::make_shared<Coordinator>();
			}
			return Instance;
		};
		Coordinator() { Init(); };
	private:
		void Init();
		
	public:
		Entity CreateEntity();
		void DestroyEntity(Entity InEntity);

		template<typename T>
		void RegisterComponent()
		{
			ComponentMgr->RegisterComponent<T>();
		}

		template<typename T>
		void AddComponent(Entity InEntity, T Component)
		{
			ComponentMgr->AddComponent<T>(InEntity, Component);
			auto Signature = EntityMgr->GetSignature(InEntity);
			Signature.set(ComponentMgr->GetComponentType<T>(), true);
			EntityMgr->SetSignature(InEntity, Signature);
			SystemMgr->EntitySignatureChanged(InEntity, Signature);
		}

		template<typename T>
		void RemoveComponent(Entity InEntity)
		{
			ComponentMgr->RemoveComponent<T>(InEntity);
			auto Signature = EntityMgr->GetSignature(InEntity);
			EntityMgr->SetSignature(InEntity, Signature);
			SystemMgr->EntitySignatureChanged(InEntity, Signature);
		}

		template<typename T>
		T& GetComponent(Entity InEntity)
		{
			return ComponentMgr->GetComponent<T>(InEntity);
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			return ComponentMgr->GetComponentType<T>();
		}

		template<typename T>
		std::shared_ptr<T> RegisterSystem(T System)
		{
			return SystemMgr->RegisterSystem<T>(System);
		}

		template<typename T>
		void SetSystemSignature(Signature Signature)
		{
			SystemMgr->SetSignature<T>(Signature);
		}

		void RunSystems(float dt)
		{
			SystemMgr->RunSystems(dt);
		}

		void InitSystems()
		{
			SystemMgr->InitSystems();
		}

	private:
		std::unique_ptr<EntityManager> EntityMgr;
		std::unique_ptr<ComponentManager> ComponentMgr;
		std::unique_ptr<SystemManager> SystemMgr;
		static std::shared_ptr<Coordinator> Instance;
	};
}

