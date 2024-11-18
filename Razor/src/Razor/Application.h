#pragma once

#include "Core.h"

namespace Razor
{
	class   Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run();
	};

	// DEFINED IN CLIENT
	Application* CreateApplication();
}

