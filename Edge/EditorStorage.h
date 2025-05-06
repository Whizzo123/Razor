#pragma once

#include <Razor.h>

namespace EdgeEditor
{
	struct EditorStorage
	{
		EditorStorage() : SelectedEntity(nullptr) {}
		Razor::Ref<Razor::Entity> SelectedEntity;
		Razor::Model DefaultModel;
	};
}

