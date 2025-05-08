#pragma once

#include "../Core.h"
#include "System.h"
#include "../Coordinator.h"

namespace Razor
{

	class   SpinSystem : public System
	{

	public:

		SpinSystem(Ref<Razor::Scene> Scene) : System(Scene)
		{
		}
		void Run(float dt) override;
	};

}