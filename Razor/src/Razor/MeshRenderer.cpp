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
            InitMesh(EntityMesh.Model->GetModelMeshData());
        }
    }

    void MeshRenderer::InitMesh(std::vector<MeshData>& Meshes)
    {
        // TODO Needs moved to inside RendererAPI
        for (MeshData& Mesh : Meshes)
        {
            glGenVertexArrays(1, &Mesh.VAO);
            glGenBuffers(1, &Mesh.VBO);
            glGenBuffers(1, &Mesh.EBO);

            glBindVertexArray(Mesh.VAO);
            glBindBuffer(GL_ARRAY_BUFFER, Mesh.VBO);

            glBufferData(GL_ARRAY_BUFFER, Mesh.Vertices.size() * sizeof(MeshData::Vertex), &Mesh.Vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh.EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, Mesh.Indices.size() * sizeof(unsigned int), &Mesh.Indices[0], GL_STATIC_DRAW);

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
            
            if (Mesh.Meshes.size() > 0)
            {
                InitMesh(Mesh.Meshes);
            }
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
