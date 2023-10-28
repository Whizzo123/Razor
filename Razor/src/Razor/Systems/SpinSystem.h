#pragma once

#include "../Core.h"
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{

	class RAZOR_API SpinSystem : public System
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