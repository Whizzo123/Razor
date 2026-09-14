#pragma once

#include "Gui/IEditorWindow.h"
#include "Razor.h"

namespace EdgeEditor
{
	struct EditorStorage;
	class AssetPickerPopupWindow;

	class Inspector : public IEditorWindow
	{
	public:
		Inspector();
		Inspector(Razor::Ref<EditorStorage> Storage);
		virtual ~Inspector() override = default;
		
		EditorWindowName GetName() override { return EditorWindowName::INSPECTOR; }
		void Render() override;

	private:
		void CreateEntity();
		void CreateWidgetForProperty(const std::string& PropertyName, const std::string& PropertyValue);

	private:
		Razor::Ref<EditorStorage> Storage;
		Razor::Ref<AssetPickerPopupWindow> _mAssetPickerPopup = nullptr;
	};
}

