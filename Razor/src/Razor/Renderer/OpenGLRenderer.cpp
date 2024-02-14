#include "OpenGLRenderer.h"
#include "../Log.h"

namespace Razor
{

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRenderer::BindFrameBuffer(unsigned int BufferIndex)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, BufferIndex);
    }

    unsigned int OpenGLRenderer::CreateFrameBuffer()
    {
        unsigned int Framebuffer;
        glGenFramebuffers(1, &Framebuffer);
        return Framebuffer;
    }

    unsigned int OpenGLRenderer::CreateTextureForFrameBuffer(unsigned int Framebuffer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
        unsigned int TextureColorBuffer;
        glGenTextures(1, &TextureColorBuffer);
        glBindTexture(GL_TEXTURE_2D, TextureColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureColorBuffer, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return TextureColorBuffer;
    }

    void OpenGLRenderer::SwapBuffer(Window& window)
    {
        glfwSwapBuffers(window.GetWindowPtr());
    }
    void OpenGLRenderer::ReadPixels(unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height, unsigned char* Pixels, unsigned int Buffer)
    {
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glReadPixels(X, Y, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, Pixels);
    }
    
}