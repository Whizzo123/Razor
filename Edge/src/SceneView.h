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
		static constexpr EditorWindowName NAME = EditorWindowName::SCENEVIEW;
		EditorWindowName GetName() override { return NAME; }
		void Render() override;

	private:
		Razor::Ref<EditorStorage> Storage;
	};
}

