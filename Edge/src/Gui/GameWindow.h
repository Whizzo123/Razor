#pragma once
#include "IEditorWindow.h"
#include "../Systems/RSEditorCamera.h"
#include "../EditorCamera.h"
#include "../EditorStorage.h"
#include <Razor.h>

namespace EdgeEditor
{
    class GameWindow : public IEditorWindow
    {
    public:
        GameWindow(Razor::Ref<EditorStorage> storage);
        virtual ~GameWindow() override = default;

        EditorWindowName GetName() override { return EditorWindowName::GAMEWINDOW; }
        void Render() override;
        void ProcessInput() override;

    private:

        Razor::Ref<Razor::Framebuffer> _mGameBuffer; /** Framebuffer object for rendering scene */
	    bool bGameViewHovered  = false; /** True when cursor hovers the Game viewport panel */
	    Razor::Vector2 _mViewportSize { 0.0f, 0.0f }; /** 2D vector to hold size of Game viewport panel */
        Razor::RenderPipelineConfig _mGamePipelineConfig;
        Razor::Ref<EditorStorage> _mStorage;
   };
}