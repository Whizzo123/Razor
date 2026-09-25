#pragma once
#include <unordered_map>
#include "IEditorWindow.h"
#include "../EditorStorage.h"
#include "Razor.h"

typedef std::unordered_map<EdgeEditor::EditorWindowName, Razor::Scope<EdgeEditor::IEditorWindow>> Layout;

namespace EdgeEditor
{
    class EditorLayout
    {
    public:
        EditorLayout(const std::string& layoutFilePath, Razor::Ref<EditorStorage> storage);
        ~EditorLayout();

        const Layout& GetWindows();
    private:
        void LoadLayout(Razor::Ref<EditorStorage> storage);
        void SaveLayout();

        Layout _mWindows;
        std::string _mFilePath;
    };
}