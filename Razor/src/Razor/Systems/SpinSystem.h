#pragma once

#include "../Core.h"
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{

	class   SpinSystem : public System
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

}