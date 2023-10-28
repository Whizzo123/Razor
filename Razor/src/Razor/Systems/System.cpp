#include "System.h"

void SystemManager::EntityDestroyed(Entity InEntity)
{
	for (auto const& Pair : Systems)
	{
		auto const& System = Pair.second;
		System->Entities.erase(InEntity);
	}
}

void SystemManager::EntitySignatureChanged(Entity InEntity, Signature EntitySignature)
{
	for (auto const& Pair : Systems)
	{
		auto const& Type = Pair.first;
		auto const& System = Pair.second;
		auto const& SystemSignature = Signatures[Type];

		// Entity signature matches system signature - insert into set
		if ((EntitySignature & SystemSignature) == SystemSignature)
		{
			System->Entities.insert(InEntity);
		}
		// Entity signature does not match system signature - erase from set
		else
		{
			System->Entities.erase(InEntity);
		}
	}
}

void SystemManager::RunSystems(float dt)
{
	for (auto const& Pair : Systems)
	{
		auto const& System = Pair.second;
		
		System->Run(dt);
	}
}

void SystemManager::InitSystems()
{
	for (auto const& Pair : Systems)
	{
		auto const& System = Pair.second;

		System->Init();
	}
}
