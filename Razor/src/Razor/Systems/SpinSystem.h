#pragma once

#include "System.h"
#include "../Coordinator.h"

class SpinSystem : public System
{

public:

	SpinSystem()
	{
		Coordinator = Razor::Coordinator::GetInstance();
	}
	void Run(float dt) override;

private:
	std::shared_ptr<Razor::Coordinator> Coordinator;
};

