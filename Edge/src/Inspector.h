#pragma once

#include "Razor.h"


namespace EdgeEditor
{
	class Inspector
	{
	public:
		void Render();

	private:
		void CreateEntity();
		void CreateWidgetForProperty(const std::string& PropertyName, const std::string& PropertyValue);
	};
}

