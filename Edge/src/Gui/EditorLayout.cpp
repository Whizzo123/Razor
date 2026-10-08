#include "EditorLayout.h"
#include "IEditorWindow.h"
#include "Layout/EditorWindowRegistry.h"
#include <fstream>


namespace EdgeEditor
{
    EditorLayout::EditorLayout(const std::string& layoutFilePath, Razor::Ref<EditorStorage> storage) : _mFilePath(layoutFilePath)
    {
        LoadLayout(storage);
    }

    EditorLayout::~EditorLayout()
    {
        SaveLayout();
    }

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
        for (auto node : Razor::yaml_get_children(windows))
        {
            int windowId = Razor::yaml_as_int(node, -1);
            EditorWindowName name = static_cast<EditorWindowName>(windowId);
            _mWindows.insert({name, EditorWindowRegistry::Instance().Create(name, storage)});
        }
    }

    void EditorLayout::SaveLayout()
    {
        Razor::YamlEmitter* Out = Razor::yaml_emitter_new();
        Razor::yaml_emitter_begin_map(Out);
        Razor::yaml_emitter_key(Out, "Windows");
        Razor::yaml_emitter_value_seq(Out);
        for (const auto& [name, window] : _mWindows)
        {
            Razor::yaml_emitter_value_int(Out, static_cast<int>(name));
        }

        Razor::yaml_emitter_end_seq(Out);
        Razor::yaml_emitter_end_map(Out);
        std::ofstream FOut(_mFilePath.c_str());
        FOut << yaml_emitter_cstr(Out);
        FOut.close();
    }

    const Layout& EditorLayout::GetWindows()
    {
        return _mWindows;
    }

    bool EditorLayout::Contains(EditorWindowName name)
    {
        return _mWindows.contains(name);
    }

    IEditorWindow* EditorLayout::GetWindow(EditorWindowName name)
    {
        if(!Contains(name))
        {
            return nullptr;
        }
        return _mWindows[name].get();
    }
}

