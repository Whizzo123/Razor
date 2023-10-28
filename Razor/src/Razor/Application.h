#pragma once

#include "Core.h"

namespace Razor
{
	class RAZOR_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// DEFINED IN CLIENT
	Application* CreateApplication();
}

