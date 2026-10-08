#pragma once

#include "EditorStorage.h"
#include "Gui/IEditorWindow.h"
namespace EdgeEditor
{
	class SystemView : public IEditorWindow
	{
	public:
		SystemView();
		SystemView(Razor::Ref<EditorStorage> storage);
		virtual ~SystemView() override = default;
		static constexpr EditorWindowName NAME = EditorWindowName::SYSTEMVIEW;
		EditorWindowName GetName() override { return NAME; }
		void Render() override;

	private:
		Razor::Ref<EditorStorage> _mStorage;
	};
}

