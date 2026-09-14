#pragma once

namespace EdgeEditor
{
    enum EditorWindowName
    {
        INSPECTOR,
        SCENEVIEW,
        SYSTEMVIEW,
        PROJECTEXPLORER,
        SCENEWINDOW,
        GAMEWINDOW
    };

    class IEditorWindow
    {
    public:
        virtual ~IEditorWindow() = default;

        virtual EditorWindowName GetName() = 0;
        virtual void Render() = 0;
        virtual void ProcessInput() {}
    };
}