#pragma once
#include <set>
#include "../Core.h"
#include "../EntityManager.h"
#include "../ComponentManager.h"

namespace Razor
{

	class RAZOR_API System
	{
	public:
		std::set<Entity> Entities;
		virtual void Init() {};
		virtual void Run(float dt) {};
	};

	class RAZOR_API SystemManager
	{
	public:

		SystemManager() {};

		template<typename T>
		std::shared_ptr<T> RegisterSystem(T SystemInst)
		{
			const char* typeName = typeid(T).name();
			assert(Systems.find(typeName) == Systems.end() && "Registering system twice");
			auto SystemInstPtr = std::make_shared<T>(SystemInst);
			Systems.insert({ typeName, SystemInstPtr });
			return SystemInstPtr;
		}

		template<typename T>
		void SetSignature(Signature InSignature)
		{
			const char* typeName = typeid(T).name();
			assert(Systems.find(typeName) != Systems.end() && "System does not exist");
			Signatures.insert({ typeName, InSignature });
		}

		void EntityDestroyed(Entity InEntity);
		void EntitySignatureChanged(Entity InEntity, Signature EntitySignature);
		void RunSystems(float dt);
		void InitSystems();
	private:
		std::unordered_map<const char*, Signature> Signatures{};
		std::unordered_map<const char*, std::shared_ptr<System>> Systems{};
	};

}