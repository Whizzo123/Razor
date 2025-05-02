#pragma once

#include <Razor.h>
#include "../EditorStorage.h"

namespace EdgeEditor
{
	class SceneView
	{
	public:
		SceneView();
		SceneView(Razor::Ref<EditorStorage> Storage);

		void Render();

	private:
		Razor::Ref<EditorStorage> Storage;
	};
}

