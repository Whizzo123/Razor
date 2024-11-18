#include "OpenGLRenderer.h"
#include "../Log.h"
#include "../../Platform/OpenGL/OpenGLWindowProvider.h"
#include "../Assert.h"
#include "OpenGL/GLFramebuffer.h"

namespace Razor
{

    void OpenGLRenderer::InitRendererAPI()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        StencilActionTranslation = std::unordered_map<EStencilAction, GLenum> 
        {
            {EStencilAction::KEEP, GL_KEEP},
            {EStencilAction::REPLACE, GL_REPLACE}
        };

        StencilFuncTranslation = std::unordered_map<EStencilFunc, GLenum>
        {
            {EStencilFunc::ALWAYS, GL_ALWAYS}
        };
    }

    void OpenGLRenderer::PollForEvents()
    {
        glfwPollEvents();
    }

    void OpenGLRenderer::TerminateRendererAPI()
    {
        glfwTerminate();
    }

    void OpenGLRenderer::EnableDepthTesting(bool bEnable)
    {
        bEnable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    }

    void OpenGLRenderer::EnableStencilTesting(bool bEnable)
    {
        bEnable ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
    }

    void OpenGLRenderer::SetStencilOperation(EStencilAction StencilFailAction, EStencilAction DepthFailAction, EStencilAction SDPassAction)
    {
        glStencilOp(Translate(StencilFailAction), Translate(DepthFailAction), Translate(SDPassAction));
    }

    GLenum OpenGLRenderer::Translate(EStencilAction Action)
    {
        RZ_CORE_ASSERT(StencilActionTranslation.find(Action) != StencilActionTranslation.end(), "Can't translate this action");
        return StencilActionTranslation[Action];
    }

    GLenum OpenGLRenderer::Translate(EStencilFunc Action)
    {
        RZ_CORE_ASSERT(StencilFuncTranslation.find(Action) != StencilFuncTranslation.end(), "Can't translate this function");
        return StencilFuncTranslation[Action];
    }

    void OpenGLRenderer::SetStencilFunction(EStencilFunc Function, unsigned int Ref, unsigned int Mask)
    {
        glStencilFunc(Translate(Function), Ref, Mask);
    }

    void OpenGLRenderer::EnableStencilWriting(bool bEnable)
    {
        bEnable ? glStencilMask(0xFF) : glStencilMask(0x00);
    }

    CameraInfo OpenGLRenderer::GetCameraInfo()
    {
        // Swap these out for actual grabbed values from potentially window class
        const int SCREEN_WIDTH = 800;
        const int SCREEN_HEIGHT = 600;

        CameraInfo Info;
        glm::mat4 View = glm::mat4(1.0f);
        glm::mat4 Projection = glm::mat4(1.0f);
        Projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        View = glm::lookAt(RendererCamera.CameraPos, RendererCamera.CameraPos + RendererCamera.CameraFront, RendererCamera.CameraUp);
        Info.CameraView = View;
        Info.CameraProjection = Projection;
        Info.CameraPosition = RendererCamera.CameraPos;
        return Info;
    }

    void OpenGLRenderer::DrawMesh(RendererInfo Info)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindVertexArray(Info.ObjMesh.VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(Info.ObjMesh.Indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void OpenGLRenderer::ClearBuffer()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void OpenGLRenderer::BindFrameBuffer(uint32_t BufferIndex)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, BufferIndex);
    }

    Ref<Framebuffer> OpenGLRenderer::CreateFrameBuffer(uint32_t Width, uint32_t Height)
    {
        Ref<Framebuffer> Buffer = CreateRef<GLFramebuffer>();
        Buffer->Refresh(Width, Height);
        return Buffer;
    }

    void OpenGLRenderer::SwapBuffer(Window& window)
    {
        glfwSwapBuffers(std::dynamic_pointer_cast<OpenGLWindowProvider>(window.GetWindowProvider())->GetPlatformWindowPtr());
    }

    void OpenGLRenderer::ReadPixels(unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height, unsigned char* Pixels, unsigned int Buffer)
    {
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glReadPixels(X, Y, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, Pixels);
    }

    void OpenGLRenderer::BackupContext()
    {
        GLFWBackupContext = glfwGetCurrentContext();
    }

    void OpenGLRenderer::ResetCurrentContext()
    {
        if(GLFWBackupContext != nullptr)
        { 
            glfwMakeContextCurrent(GLFWBackupContext);
        }
        else
        {
            RZ_CORE_ERROR("Failed to reset current context 'GLFWBackupContext' == nullptr");
        }
    }

    void* OpenGLRenderer::GetSceneRenderedToTexture()
    {
        return (void*)(GLuint)SceneTexture;
    }

    void OpenGLRenderer::SetFrameBufferTextureForScene(unsigned int InSceneTexture)
    {
        SceneTexture = InSceneTexture;
    }

    void OpenGLRenderer::ResizeFramebuffers(unsigned int Width, unsigned int Height)
    {
        //There are things that depend on these framebuffers so how do we notify them that the framebuffers have been regenerated and they need to grab the new ones
        //How do we know what framebuffers to regenerate?
        //Do we keep a list of these framebuffers and there relevant textures a new type?
    }
    
}