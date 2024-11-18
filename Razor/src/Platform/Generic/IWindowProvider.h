#pragma once
#include <string>

namespace Razor
{
	class IWindowProvider
	{
	public:
		virtual void CreateProviderWindow(int Width, int Height, const std::string& Title) = 0;
		virtual bool ShouldWindowClose() = 0;
		virtual void SetWindowToClose() = 0;
	};
}

