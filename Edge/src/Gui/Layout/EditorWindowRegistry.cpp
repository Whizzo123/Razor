#include "EditorWindowRegistry.h"

namespace EdgeEditor
{

    Razor::Scope<EditorWindowRegistry> EditorWindowRegistry::_mInstance = nullptr;

    EditorWindowRegistry::EditorWindowRegistry()
    {

    }

    EditorWindowRegistry& EditorWindowRegistry::Instance()
    {
        if (!_mInstance)
        {
            _mInstance = Razor::Scope<EditorWindowRegistry>(new EditorWindowRegistry());
        }
        return *_mInstance;
    }

    Razor::Scope<IEditorWindow> EditorWindowRegistry::Create(EditorWindowName name, Razor::Ref<EditorStorage> storage)
    {
        return _mCreateFuncs.at(name)(storage);
    }

    void EditorWindowRegistry::Register(EditorWindowName name, CreateFunc func)
    {
        _mCreateFuncs.insert({name, func});
    }



}