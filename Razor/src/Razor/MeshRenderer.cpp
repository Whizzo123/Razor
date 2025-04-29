#include "MeshRenderer.h"
#include "TextureReader.h"
#include "Renderer/Shaders/DefaultMeshShader.h"




namespace Razor
{
    void MeshRenderer::Init()
    {
        auto View = CurrentScene->GetEntitiesWithComponents<Mesh>();
        for (auto Entity : View)
        {
            Mesh& EntityMesh = CurrentScene->GetComponent<Mesh>(Entity);
            for (MeshData& Mesh : EntityMesh.Model->GetModelMeshData())
            {
                if (Mesh.Vertices.size() > 0)
                {
                    InitMesh(Mesh);
                }
            } 
        }
    }

    void MeshRenderer::InitMesh(MeshData& MeshToInit)
    {
        // TODO Needs moved to inside RendererAPI
        glGenVertexArrays(1, &MeshToInit.VAO);
        glGenBuffers(1, &MeshToInit.VBO);
        glGenBuffers(1, &MeshToInit.EBO);

        glBindVertexArray(MeshToInit.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, MeshToInit.VBO);

        glBufferData(GL_ARRAY_BUFFER, MeshToInit.Vertices.size() * sizeof(MeshData::Vertex), &MeshToInit.Vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MeshToInit.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, MeshToInit.Indices.size() * sizeof(unsigned int), &MeshToInit.Indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshData::Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshData::Vertex), (void*)offsetof(MeshData::Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshData::Vertex), (void*)offsetof(MeshData::Vertex, TexCoords));

        glBindVertexArray(0);
        for (MeshData& Child : MeshToInit.Meshes)
        {
            InitMesh(Child);
        }
    }

    void MeshRenderer::Run(float dt)
    {
       /* CameraInfo CamInfo = Renderer->GetCameraInfo();
        for (auto const& Entity : Entities)
        {
            Mesh EntityMesh = Coordinator->GetComponent<Mesh>(Entity);
            Transform EntityTransform = Coordinator->GetComponent<Transform>(Entity);
            Material EntityMat = Coordinator->GetComponent<Material>(Entity);

            for (const MeshData& Child : EntityMesh.Data)
            {
                glUseProgram(EntityMat.ShaderID);
                const MaterialData& MeshMaterial = EntityMat.Materials.at(Child.MaterialId);
                ShaderMap[EntityMat.ShaderID]->Setup(&CamInfo, EntityTransform.Get(), MeshMaterial, Lights);
                Renderer->DrawMesh({Child});
            }   
        }*/
    }
}
