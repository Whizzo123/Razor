#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Razor
{

	void Model::LoadMesh(std::string Path)
	{
		Assimp::Importer Import;
		const aiScene* Scene = Import.ReadFile(Path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
		{
			RZ_CORE_ERROR("ERROR::ASSIMP::{0}", Import.GetErrorString());
			Info = ModelInfo();
			return;
		}
		Directory = Path.substr(0, Path.find_last_of('/'));

		std::vector<MeshData> Meshes;

		ProcessNode(Scene->mRootNode, Scene, Meshes);

		ModelInfo ProcessedInfo;
		ProcessedInfo.ModelMeshData = Meshes;
		Material ReturnMaterial;
		ReturnMaterial.Materials = Materials_Loaded;
		ProcessedInfo.ModelMaterial = ReturnMaterial;

		Info = ProcessedInfo;
	}

	void Model::ProcessNode(aiNode* Node, const aiScene* Scene, std::vector<MeshData>& Parts)
	{
		for (unsigned int i = 0; i < Node->mNumMeshes; i++)
		{
			aiMesh* Mesh = Scene->mMeshes[Node->mMeshes[i]];
			Parts.push_back(ProcessMesh(Mesh, Scene));
		}

		for (unsigned int i = 0; i < Node->mNumChildren; i++)
		{
			ProcessNode(Node->mChildren[i], Scene, Parts);
		}
	}

	MeshData Model::ProcessMesh(aiMesh* MeshToProcess, const aiScene* Scene)
	{
		// data to fill
		std::vector<MeshData::Vertex> Vertices;
		std::vector<unsigned int> Indices;
		std::vector<MeshData::Texture> Textures;

		// walk through each of the mesh's vertices
		for (unsigned int i = 0; i < MeshToProcess->mNumVertices; i++)
		{
			MeshData::Vertex Vertex;
			glm::vec3 Vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// positions
			Vector.x = MeshToProcess->mVertices[i].x;
			Vector.y = MeshToProcess->mVertices[i].y;
			Vector.z = MeshToProcess->mVertices[i].z;
			Vertex.Position = Vector;
			// normals
			if (MeshToProcess->HasNormals())
			{
				Vector.x = MeshToProcess->mNormals[i].x;
				Vector.y = MeshToProcess->mNormals[i].y;
				Vector.z = MeshToProcess->mNormals[i].z;
				Vertex.Normal = Vector;
			}
			// texture coordinates
			if (MeshToProcess->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 Vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				Vec.x = MeshToProcess->mTextureCoords[0][i].x;
				Vec.y = MeshToProcess->mTextureCoords[0][i].y;
				Vertex.TexCoords = Vec;

				//// tangent
				//Vector.x = MeshToProcess->mTangents[i].x;
				//Vector.y = MeshToProcess->mTangents[i].y;
				//Vector.z = MeshToProcess->mTangents[i].z;
				//Vertex.Tangent = Vector;
				//// bitangent
				//Vector.x = MeshToProcess->mBitangents[i].x;
				//Vector.y = MeshToProcess->mBitangents[i].y;
				//Vector.z = MeshToProcess->mBitangents[i].z;
				//Vertex.Bitangent = Vector;
			}
			else
				Vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			Vertices.push_back(Vertex);
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < MeshToProcess->mNumFaces; i++)
		{
			aiFace face = MeshToProcess->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				Indices.push_back(face.mIndices[j]);
		}
		// process materials
		aiMaterial* material = Scene->mMaterials[MeshToProcess->mMaterialIndex];
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1. diffuse maps
		std::vector<MeshData::Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		Textures.insert(Textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<MeshData::Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		Textures.insert(Textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<MeshData::Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		Textures.insert(Textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<MeshData::Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		Textures.insert(Textures.end(), heightMaps.begin(), heightMaps.end());

		unsigned int MaterialID = LoadMaterialParam(material);

		// return a mesh object created from the extracted mesh data
		return MeshData(Vertices, Indices, Textures, MaterialID);
	}

	std::vector<MeshData::Texture> Model::LoadMaterialTextures(aiMaterial* Mat, aiTextureType Type, std::string TypeName)
	{
		std::vector<MeshData::Texture> textures;
		for (unsigned int i = 0; i < Mat->GetTextureCount(Type); i++)
		{
			aiString str;
			Mat->GetTexture(Type, i, &str);
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for (unsigned int j = 0; j < Textures_Loaded.size(); j++)
			{
				if (std::strcmp(Textures_Loaded[j].Path.data(), str.C_Str()) == 0)
				{
					textures.push_back(Textures_Loaded[j]);
					skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // if texture hasn't been loaded already, load it
				MeshData::Texture texture;
				texture.Id = TextureFromFile(str.C_Str());
				texture.Type = TypeName;
				texture.Path = str.C_Str();
				textures.push_back(texture);
				Textures_Loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
			}
		}
		return textures;
	}

	unsigned int Model::LoadMaterialParam(aiMaterial* Mat)
	{
		aiString Name("");
		Mat->Get(AI_MATKEY_NAME, Name);
		for (unsigned int i = 0; i < Materials_Loaded.size(); i++)
		{
			if (std::strcmp(Materials_Loaded[i].MaterialName.c_str(), Name.C_Str()) == 0)
			{
				return i;
			}
		}
		aiColor3D Diffuse(0.0f, 0.0f, 0.0f);
		Mat->Get(AI_MATKEY_COLOR_DIFFUSE, Diffuse);
		aiColor3D Ambient(0.0f, 0.0f, 0.0f);
		Mat->Get(AI_MATKEY_COLOR_AMBIENT, Ambient);
		aiColor3D Specular(0.0f, 0.0f, 0.0f);
		Mat->Get(AI_MATKEY_COLOR_SPECULAR, Specular);
		float Shininess = 0.0f;
		Mat->Get(AI_MATKEY_SHININESS, Shininess);
		MaterialData Data(glm::vec3(Ambient.r, Ambient.g, Ambient.b), glm::vec3(Diffuse.r, Diffuse.g, Diffuse.b), glm::vec3(Specular.r, Specular.g, Specular.b), Shininess);
		Data.MaterialName = std::string(Name.C_Str());
		Materials_Loaded.push_back(Data);
		return Materials_Loaded.size() - 1;
	}

	unsigned int Model::TextureFromFile(const char* Path)
	{
		std::string Filename = std::string(Path);
		Filename = Directory + '/' + Filename;

		unsigned int TextureID;
		glGenTextures(1, &TextureID);

		int Width, Height, NrComponents;
		unsigned char* Data = stbi_load(Filename.c_str(), &Width, &Height, &NrComponents, 0);
		if (Data)
		{
			GLenum format;
			if (NrComponents == 1)
				format = GL_RED;
			else if (NrComponents == 3)
				format = GL_RGB;
			else if (NrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, TextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, Width, Height, 0, format, GL_UNSIGNED_BYTE, Data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(Data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << Path << std::endl;
			stbi_image_free(Data);
		}

		return TextureID;
	}

	std::vector<MeshData>& Model::GetModelMeshData()
	{
		return Info.ModelMeshData;
	}

}