#pragma once

#include "Gui/IEditorWindow.h"
namespace EdgeEditor
{
	class SystemView : public IEditorWindow
	{
	public:
		SystemView() = default;
		virtual ~SystemView() override = default;

		EditorWindowName GetName() override { return EditorWindowName::SYSTEMVIEW; }
		void Render() override;
	};
}

