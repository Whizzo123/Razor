#pragma once
#include "../Core.h"
#include "../Component.h"
#include <assimp/mesh.h>
#include <assimp/scene.h>

namespace Razor
{
	struct ModelInfo
	{
		Mesh ModelMesh;
		Material ModelMaterial;
	};

	class   Model
	{
	public:
		ModelInfo LoadMesh(std::string Path);
		void ProcessNode(aiNode* Node, const aiScene* Scene, std::vector<MeshData>& Parts);
		MeshData ProcessMesh(aiMesh* Mesh, const aiScene* Scene);
		std::vector<MeshData::Texture> LoadMaterialTextures(aiMaterial* Mat, aiTextureType Type, std::string TypeName);
		unsigned int LoadMaterialParam(aiMaterial* Mat);
		unsigned int TextureFromFile(const char* Path);
		std::vector<MaterialData> GetMaterialsForModel() { return Materials_Loaded; };
	private:
		std::vector<MeshData::Texture> Textures_Loaded;
		std::vector<MaterialData> Materials_Loaded;
		std::string Directory;
	};
}

