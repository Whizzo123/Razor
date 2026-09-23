#pragma once

#include <unordered_map>
#include "../IEditorWindow.h"
#include "Razor.h"

namespace EdgeEditor
{
    class EditorWindowRegistry
    {
    public:
        using CreateFunc = std::function<Razor::Scope<IEditorWindow>()>;

        static EditorWindowRegistry& Instance();
        void Register(EditorWindowName name, CreateFunc func);
        Razor::Scope<IEditorWindow> Create(EditorWindowName name);
    private:
        EditorWindowRegistry();
        static Razor::Scope<EditorWindowRegistry> _mInstance;

        std::unordered_map<EditorWindowName, CreateFunc> _mCreateFuncs;
    };

    template<typename T>
    void RegisterEditorWindow()
    {
        EditorWindowRegistry::Instance().Register(T::NAME, [](){ return Razor::CreateScope<T>(); });
    }
}