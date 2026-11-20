#pragma once
#include "../Core.h"
#include <vector>
#include "Textures/Texture.h"
#include "../Utils/Vector3.h"
#include "../Utils/Vector2.h"

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

namespace Razor
{

	struct MeshData
	{
		struct Vertex
		{
			Vector3 Position;
			Vector3 Normal;
			Vector2 TexCoords;
			Vector3 Tangent;
			Vector3 Bitangent;
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
			: Vertices(Vertices),
			  Indices(Indices), 
			  Textures(Textures), 
			  MaterialId(MaterialID)
		{
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
		Vector3 Ambient;
		Vector3 Diffuse;
		Vector3 Specular;
		float Shininess;
		MaterialData() : MaterialName ("Untitled"), Ambient(1.0f, 1.0f, 1.0f), Diffuse(1.0f, 1.0f, 1.0f), Specular(1.0f, 1.0f, 1.0f), Shininess(1.0f) {}
		MaterialData(Vector3 A, Vector3 D, Vector3 S, float SH) : Ambient(A), Diffuse(D), Specular(S), Shininess(SH) {}
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

	class RAZOR_API Model
	{
	public:
		Model();
		Model(const std::string& path);
		void LoadMesh(std::string Path);
		void ProcessNode(aiNode* Node, const aiScene* Scene, std::vector<MeshData>& Parts);
		MeshData ProcessMesh(aiMesh* Mesh, const aiScene* Scene);
		std::vector<MeshData::Texture> LoadMaterialTextures(aiMaterial* Mat, TextureType Type, std::string TypeName);
		unsigned int LoadMaterialParam(aiMaterial* Mat);
		unsigned int TextureFromFile(const char* Path);
		std::vector<MaterialData> GetMaterialsForModel() { return Materials_Loaded; };
		void SetModelShader(uint8_t ID) { Info.ModelMaterial.ShaderID = ID; }
		Material& GetMaterial() { return Info.ModelMaterial; }

		std::vector<MeshData>& GetModelMeshData();
		const ModelInfo& GetModelInfo() const { return Info; }
		void SetModelInfo(const ModelInfo& Info) { this->Info = Info; }

		const std::string& GetName() const { return _mName; }
		/**
		* Serialize a Model object and save to the provided path in YAML format
		*
		* @param path The path which we will save the YAML file to
		*/
		void Serialize(const std::string& path) const;
	private:
		/**
		* Deserialize a Model object from the provided path
		*
		* @param path The path which we will load the YAML file from
		*/
		void Deserialize(const std::string& path);
		std::vector<MeshData::Texture> Textures_Loaded;
		std::vector<MaterialData> Materials_Loaded;
		std::string Directory;
		std::string _mName;
		ModelInfo Info;
	};
}

