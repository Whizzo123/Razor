#pragma once
#include <string>
#include "../../Razor/Core.h"

namespace Razor
{
	class RAZOR_API IWindowProvider
	{
	public:
		virtual ~IWindowProvider() = default;
		virtual void CreateProviderWindow(int Width, int Height, const std::string& Title) = 0;
		virtual bool ShouldWindowClose() = 0;
		virtual void SetWindowToClose() = 0;
	};
}

