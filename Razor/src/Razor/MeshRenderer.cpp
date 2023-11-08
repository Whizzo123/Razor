#include "MeshRenderer.h"
#include "TextureReader.h"
#include "Renderer/Shaders/DefaultMeshShader.h"




namespace Razor
{
    void MeshRenderer::Init()
    {
        MeshShader = std::make_shared<DefaultMeshShader>();
        for (Entity Entity : Entities)
        {
            Mesh& EntityMesh = Coordinator->GetComponent<Mesh>(Entity);
            for (MeshData& Mesh : EntityMesh.Data)
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
        // Still not reading any data in renderdoc got to be something stupid
        CameraInfo CamInfo = Renderer->GetCameraInfo();
        for (auto const& Entity : Entities)
        {
            Mesh EntityMesh = Coordinator->GetComponent<Mesh>(Entity);
            Transform EntityTransform = Coordinator->GetComponent<Transform>(Entity);
            Material EntityMat = Coordinator->GetComponent<Material>(Entity);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            for (const MeshData& Child : EntityMesh.Data)
            {
                glUseProgram(MeshShader->ID);
                MeshShader->Setup(CamInfo.CameraProjection, CamInfo.CameraView, EntityTransform.Get(), glm::vec3(1.0f, 1.0f, 1.0f));
                int number = Child.Vertices.size();
                // SUpposedly are not drawing anymore certainly nothing that renderdoc can pick up
                const MaterialData& MeshMaterial = EntityMat.Materials.at(Child.MaterialId);
                //attempt to pass in 4 textures and 1 material then pass bool for if we have textures or not
                MeshShader->SetInt("material.texture_diffuse", 0);
                MeshShader->SetInt("material.texture_specular", 0);
                MeshShader->SetInt("material.texture_normal", 0);
                MeshShader->SetInt("material.texture_height", 0);
                MeshShader->SetVec3("material.diffuse", MeshMaterial.Diffuse);
                MeshShader->SetVec3("material.ambient", MeshMaterial.Ambient);
                MeshShader->SetVec3("material.specular", MeshMaterial.Specular);
                MeshShader->SetFloat("material.shininess", MeshMaterial.Shininess);
                MeshShader->SetBool("material.bUseTextures", 0);
                // Light  ->
                MeshShader->SetVec3("light.position", glm::vec3(20.0f, 1.0f, 10.0f));
                MeshShader->SetVec3("light.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
                MeshShader->SetVec3("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
                MeshShader->SetVec3("light.specular", glm::vec3(0.0f, 0.0f, 0.0f));
                // Camera ->
                MeshShader->SetVec3("viewPos", CamInfo.CameraPosition);
                Renderer->DrawMesh({Child, *MeshShader});
            }   
        }
    }
}
