#include "EditorLayout.h"
#include "IEditorWindow.h"
#include "Layout/EditorWindowRegistry.h"


namespace EdgeEditor
{
    EditorLayout::EditorLayout(const std::string& layoutFilePath, Razor::Ref<EditorStorage> storage) : _mFilePath(layoutFilePath)
    {
        LoadLayout(storage);
    }

    EditorLayout::~EditorLayout()
    {

    }

    /*
    Windows:
        Inspector
        Scene
        Game
        ProjectExplorer
    */

    void EditorLayout::LoadLayout(Razor::Ref<EditorStorage> storage)
    {
        Razor::YamlNode* data = Razor::yaml_load_file(_mFilePath.c_str());
        if (!data) 
        {
            RZ_ERROR("LoadLayout failed to load layout file at: {0}", _mFilePath);
            return;
        }

        Razor::YamlNode* windows = Razor::yaml_get_child(data, "Windows");

        if (!windows)
        {
            RZ_ERROR("LoadLayout failed layout file is corrupted: {0} missing 'Windows' key", _mFilePath);
        }
        for (auto node : Razor::yaml_get_children(data, "Windows"))
        {
            int windowId = Razor::yaml_as_int(node, -1);
            EditorWindowName name = static_cast<EditorWindowName>(windowId);
            _mWindows.insert({name, EditorWindowRegistry::Instance().Create(name, storage)});
        }
    }

    void EditorLayout::SaveLayout()
    {

    }

    const Layout& EditorLayout::GetWindows()
    {
        return _mWindows;
    }
}

