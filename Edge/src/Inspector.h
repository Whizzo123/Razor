#pragma once

#include "Razor.h"

namespace EdgeEditor
{
	struct EditorStorage;

	class Inspector
	{
	public:
		Inspector();
		Inspector(Razor::Ref<EditorStorage> Storage);
		void Render();

	private:
		void CreateEntity();
		void CreateWidgetForProperty(const std::string& PropertyName, const std::string& PropertyValue);

	private:
		Razor::Ref<EditorStorage> Storage;
	};
}

