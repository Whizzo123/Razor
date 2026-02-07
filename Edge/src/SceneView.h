#pragma once

#include <Razor.h>

namespace EdgeEditor
{
	struct EditorStorage;

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

