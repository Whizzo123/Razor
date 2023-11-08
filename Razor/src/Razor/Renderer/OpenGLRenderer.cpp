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
        glBindVertexArray(Info.ObjMesh.VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(Info.ObjMesh.Indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void OpenGLRenderer::DrawCube(MeshData Data, Shader CubeShader)
    {
        glBindVertexArray(Data.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

}