#pragma once
#include "../Component.h"
#include <assimp/mesh.h>
#include <assimp/scene.h>
class Model
{
public:
	Mesh LoadMesh(std::string Path);
	void ProcessNode(aiNode* Node, const aiScene* Scene, std::vector<Mesh>& Parts);
	Mesh ProcessMesh(aiMesh* Mesh, const aiScene* Scene);
	std::vector<Mesh::Texture> LoadMaterialTextures(aiMaterial* Mat, aiTextureType Type, std::string TypeName);
	unsigned int TextureFromFile(const char* Path);

private:
	std::vector<Mesh::Texture> Textures_Loaded;
	std::string Directory;
};

