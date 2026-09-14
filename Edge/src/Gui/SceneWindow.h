#pragma once
#include "IEditorWindow.h"
#include "../Systems/RSEditorCamera.h"
#include "../EditorCamera.h"
#include "../EditorStorage.h"
#include <Razor.h>

namespace EdgeEditor
{
    class SceneWindow : public IEditorWindow
    {
    public:
        SceneWindow(Razor::Ref<EditorStorage> storage);
        virtual ~SceneWindow() override = default;

        EditorWindowName GetName() override { return EditorWindowName::SCENEWINDOW; }
        void Render() override;
        void ProcessInput() override;

    private:
        /**
	    * Function to pick and object via the PickBuffer
	    * Needs it's functionality properly implemented
	    *
	    * @param PickBuffer - Shared Ptr to the FrameBuffer used as the PickBuffer
	    * @param Offset - 2D vector to offset mouse position based on viewport position
	    */
        void PickObject(Razor::Vector2 mousePos);

        Razor::Ref<Razor::Framebuffer> _mSceneBuffer; /** Framebuffer object for rendering scene */
        Razor::Ref<Razor::Framebuffer> _mPickBuffer; /** Framebuffer object for rendering scene */
        Razor::Vector2 _mViewportSize { 0.0f, 0.0f }; /** 2D vector to hold size of viewport window */
        Razor::Vector2 _mViewportPos { 0.0f, 0.0f }; /** 2D vector to hold position of image displaying scene texture for viewport*/
        Razor::RenderPipelineConfig _mEditorPipelineConfig;
        Razor::RenderPipelineConfig _mPickPipelineConfig;
        Razor::Ref<EdgeEditor::RSEditorCamera> _mEditorCameraSystem;
        EdgeEditor::EditorCamera _mEditorCamera; /** Object to house and control the camera we will use for rendering the scene to the viewport */
        bool bSceneViewHovered = false; /** True when cursor hovers the Scene viewport panel */
        Razor::Ref<EditorStorage> _mStorage;
   };
}