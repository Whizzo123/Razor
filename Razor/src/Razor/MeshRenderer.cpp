#include "MeshRenderer.h"
#include "TextureReader.h"

void MeshRenderer::Init()
{
    for (Entity Entity : Entities)
    {
        Mesh& EntityMesh = Coordinator->GetComponent<Mesh>(Entity);
        for (Mesh& Mesh : EntityMesh.Meshes)
        {
            if (Mesh.Vertices.size() > 0)
            {
                InitMesh(Mesh);
            }
        }
    }
}

void MeshRenderer::InitMesh(Mesh& MeshToInit)
{
    glGenVertexArrays(1, &MeshToInit.VAO);
    glGenBuffers(1, &MeshToInit.VBO);
    glGenBuffers(1, &MeshToInit.EBO);

    glBindVertexArray(MeshToInit.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, MeshToInit.VBO);

    glBufferData(GL_ARRAY_BUFFER, MeshToInit.Vertices.size() * sizeof(Mesh::Vertex), &MeshToInit.Vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MeshToInit.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MeshToInit.Indices.size() * sizeof(unsigned int), &MeshToInit.Indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, TexCoords));

    glBindVertexArray(0);
    for (Mesh& Child : MeshToInit.Meshes)
    {
        InitMesh(Child);
    }
}

void MeshRenderer::Run(float dt)
{
    unsigned int ShaderID = ShaderReader::CreateShaderProgram("vertex.vert", "fragment.frag");
    Shader Shader;
    Shader.ID = ShaderID;
    std::vector<unsigned int> Textures = { TextureReader::CreateTexture("test.png"), TextureReader::CreateTexture("awesomeface.png") };
    Shader.AddTextures(Textures);

	for (auto const& Entity : Entities)
	{
		Mesh EntityMesh = Coordinator->GetComponent<Mesh>(Entity);
        Transform EntityTransform = Coordinator->GetComponent<Transform>(Entity);

        Renderer->Draw({ EntityMesh, Shader, EntityTransform, EntityMesh.bHasIndices});
	}
}
