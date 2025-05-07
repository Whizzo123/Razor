#pragma once
#include "../Core.h"
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float2.hpp>

namespace Razor
{

	struct MeshData
	{
		struct Vertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
			glm::vec3 Tangent;
			glm::vec3 Bitangent;
		};
		struct Texture
		{
			unsigned int Id;
			std::string Type;
			std::string Path;
		};

		MeshData()
		{

		}

		MeshData(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, std::vector<Texture> Textures, unsigned int MaterialID)
		{
			this->Vertices = Vertices;
			this->Indices = Indices;
			this->Textures = Textures;
			MaterialId = MaterialID;
		}

		std::vector<Vertex> Vertices;
		std::vector<MeshData> Meshes;
		std::vector<unsigned int> Indices;
		std::vector<Texture> Textures;
		unsigned int VAO, VBO, EBO;
		unsigned int MaterialId;
		bool bHasIndices = true;
	};

	struct MaterialData
	{
		std::string MaterialName;
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		float Shininess;
		MaterialData() : MaterialName ("Untitled"), Ambient(glm::vec3(1.0f, 1.0f, 1.0f)), Diffuse(glm::vec3(1.0f, 1.0f, 1.0f)), Specular(glm::vec3(1.0f, 1.0f, 1.0f)), Shininess(1.0f) {}
		MaterialData(glm::vec3 A, glm::vec3 D, glm::vec3 S, float SH) : Ambient(A), Diffuse(D), Specular(S), Shininess(SH) {}
	};

	struct Material
	{
		std::vector<MaterialData> Materials;
		uint8_t ShaderID;
	};

	struct ModelInfo
	{
		std::vector<MeshData> ModelMeshData;
		Material ModelMaterial;
	};

	class Model
	{
	public:
		void LoadMesh(std::string Path);
		void ProcessNode(aiNode* Node, const aiScene* Scene, std::vector<MeshData>& Parts);
		MeshData ProcessMesh(aiMesh* Mesh, const aiScene* Scene);
		std::vector<MeshData::Texture> LoadMaterialTextures(aiMaterial* Mat, aiTextureType Type, std::string TypeName);
		unsigned int LoadMaterialParam(aiMaterial* Mat);
		unsigned int TextureFromFile(const char* Path);
		std::vector<MaterialData> GetMaterialsForModel() { return Materials_Loaded; };
		void SetModelShader(uint8_t ID) { Info.ModelMaterial.ShaderID = ID; }
		Material& GetMaterial() { return Info.ModelMaterial; }

		std::vector<MeshData>& GetModelMeshData();
		ModelInfo& GetModelInfo() { return Info; }
		void SetModelInfo(const ModelInfo& Info) { this->Info = Info; }
	private:
		std::vector<MeshData::Texture> Textures_Loaded;
		std::vector<MaterialData> Materials_Loaded;
		std::string Directory;
		ModelInfo Info;
	};
}

