#include "OpenGLRenderer.h"


void OpenGLRenderer::Draw(RendererInfo Info)
{
    Mesh ObjMesh = Info.ObjMesh;
    Shader ObjShader = Info.ObjShader;
    Transform ObjTransform = Info.ObjTransform;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    DrawMesh(ObjMesh, ObjShader, ObjTransform);
    for (Mesh Child : ObjMesh.Meshes)
    {
        DrawMesh(Child, ObjShader, ObjTransform);
    }   
}

void OpenGLRenderer::DrawMesh(Mesh& MeshToDraw, Shader& MeshShader, Transform& MeshTransform)
{
    glUseProgram(MeshShader.ID);

    MeshShader.SetTextures();

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    view = glm::lookAt(RendererCamera.CameraPos, RendererCamera.CameraPos + RendererCamera.CameraFront, RendererCamera.CameraUp);

    MeshShader.SetMat4Float("projection", projection);
    MeshShader.SetMat4Float("view", view);
    glm::mat4 result = MeshTransform.Get();
    MeshShader.SetMat4Float("model", result);

    glBindVertexArray(MeshToDraw.VAO);

    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(MeshToDraw.Indices.size()), GL_UNSIGNED_INT, 0);
   
    glBindVertexArray(0);
}