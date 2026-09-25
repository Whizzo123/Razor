#pragma once

#include <unordered_map>
#include "../IEditorWindow.h"
#include "../../EditorStorage.h"
#include "Razor.h"

namespace EdgeEditor
{
    class EditorWindowRegistry
    {
    public:
        using CreateFunc = std::function<Razor::Scope<IEditorWindow>(Razor::Ref<EditorStorage>)>;

        static EditorWindowRegistry& Instance();
        void Register(EditorWindowName name, CreateFunc func);
        Razor::Scope<IEditorWindow> Create(EditorWindowName name, Razor::Ref<EditorStorage> storage);
    private:
        EditorWindowRegistry();
        static Razor::Scope<EditorWindowRegistry> _mInstance;

        std::unordered_map<EditorWindowName, CreateFunc> _mCreateFuncs;
    };

    template<typename T>
    void RegisterEditorWindow()
    {
        EditorWindowRegistry::Instance().Register(T::NAME, [](Razor::Ref<EditorStorage> storage){ return Razor::CreateScope<T>(storage); });
    }
}