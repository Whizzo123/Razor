#pragma once

#include <Razor.h>
#include "Project.h"

namespace EdgeEditor
{
	struct EditorStorage
	{
		EditorStorage() : SelectedEntity(nullptr) {}
		Razor::Ref<Razor::Entity> SelectedEntity;
		Razor::Model DefaultModel;
		Project CurrentProject;
	};
}

