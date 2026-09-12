#pragma once

#include "Gui/IEditorWindow.h"
#include <Razor.h>

namespace EdgeEditor
{
	struct EditorStorage;

	class SceneView : public IEditorWindow
	{
	public:
		SceneView();
		SceneView(Razor::Ref<EditorStorage> Storage);
		virtual ~SceneView() override = default;

		EditorWindowName GetName() override { return EditorWindowName::SCENEVIEW; }
		void Render() override;

	private:
		Razor::Ref<EditorStorage> Storage;
	};
}

