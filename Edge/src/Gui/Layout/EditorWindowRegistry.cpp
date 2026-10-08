#include "EditorWindowRegistry.h"
#include "../GameWindow.h"
#include "../SceneWindow.h"
#include "../../Inspector.h"
#include "../../ProjectExplorer.h"
#include "../../SceneView.h"
#include "../../SystemView.h"

namespace EdgeEditor
{

    Razor::Scope<EditorWindowRegistry> EditorWindowRegistry::_mInstance = nullptr;

    EditorWindowRegistry::EditorWindowRegistry()
    {
        
    }

    void EditorWindowRegistry::Register()
    {
        RegisterEditorWindow<Inspector>();
        RegisterEditorWindow<ProjectExplorer>();
        RegisterEditorWindow<SceneView>();
        RegisterEditorWindow<SystemView>();
        RegisterEditorWindow<SceneWindow>();
        RegisterEditorWindow<GameWindow>();
    }

    EditorWindowRegistry& EditorWindowRegistry::Instance()
    {
        if (!_mInstance)
        {
            _mInstance = Razor::Scope<EditorWindowRegistry>(new EditorWindowRegistry());
            _mInstance->Register();
        }
        return *_mInstance;
    }

    Razor::Scope<IEditorWindow> EditorWindowRegistry::Create(EditorWindowName name, Razor::Ref<EditorStorage> storage)
    {
        if(!_mCreateFuncs.contains(name))
        {
            return nullptr;
        }
        return _mCreateFuncs.at(name)(storage);
    }

    void EditorWindowRegistry::Register(EditorWindowName name, CreateFunc func)
    {
        _mCreateFuncs.insert({name, func});
    }



}