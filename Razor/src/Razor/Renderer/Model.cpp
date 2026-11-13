#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <string>
#include <fstream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Log.h"
#include "../IO/YamlWrapper.h"

namespace Razor
{

	Model::Model()
	{

	}

	Model::Model(const std::string& path)
	{
		Deserialize(path);
	}

	void Model::Serialize(const std::string& path) const
	{
		YamlEmitter* out = Razor::yaml_emitter_new();
		yaml_emitter_begin_map(out);
		yaml_emitter_key(out, "ModelInfo");
		yaml_emitter_value_modelinfo(out, GetModelInfo());
		yaml_emitter_end_map(out);

		const std::string pathPlusExt = path + "/" + _mName + ".model";

		std::ofstream fOut(pathPlusExt.c_str());
		bool bFileWriteSuccessful = true;
		std::stringstream errorStream;
		if (!fOut)
		{
			bFileWriteSuccessful = false;
			errorStream << "Failed to open " << pathPlusExt;
		}
		fOut << Razor::yaml_emitter_cstr(out);
		if (!fOut)
		{
			bFileWriteSuccessful = false;
			errorStream << "Failed to write " << pathPlusExt;
		}
		fOut.close();
		if (!bFileWriteSuccessful)
		{
			RZ_CORE_WARN("Model::Serialize -> {0}", errorStream.str());
		}

	}

	void Model::Deserialize(const std::string& path)
	{
		Razor::YamlNode* data;
		const std::string pathPlusExt = path;
		try
		{
			data = yaml_load_file(pathPlusExt.c_str());
		}
		catch (std::exception e)
		{
			RZ_CORE_ERROR("Model::Deserialize -> Failed to load .model file '{0}'\n	{1}", pathPlusExt, e.what());
			return;
		}

		if (!yaml_get_child(data, "ModelInfo"))
		{
			RZ_CORE_ERROR("Model::Deserialize -> Incomplete .model file missing 'ModelInfo' key");
			return;
		}

		Razor::ModelInfo info = yaml_as_modelinfo(yaml_get_child(data, "ModelInfo"));
		SetModelInfo(info);
	}

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
		_mName = Path.substr(1, Path.find_last_of('/'));
		_mName = _mName.substr(0, Path.find_last_of('.'));

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
			Vector3 Vector; // RZ-39 TODO: allow conversion between assimp vector and our vector to make this code simpler
			// positions
			Vector.X = MeshToProcess->mVertices[i].x;
			Vector.Y = MeshToProcess->mVertices[i].y;
			Vector.Z = MeshToProcess->mVertices[i].z;
			Vertex.Position = Vector;
			// normals
			if (MeshToProcess->HasNormals())
			{
				Vector.X = MeshToProcess->mNormals[i].x;
				Vector.Y = MeshToProcess->mNormals[i].y;
				Vector.Z = MeshToProcess->mNormals[i].z;
				Vertex.Normal = Vector;
			}
			// texture coordinates
			if (MeshToProcess->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				Vector2 Vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				Vec.X = MeshToProcess->mTextureCoords[0][i].x;
				Vec.Y = MeshToProcess->mTextureCoords[0][i].y;
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
				Vertex.TexCoords = Vector2(0.0f, 0.0f);

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
		std::vector<MeshData::Texture> diffuseMaps = LoadMaterialTextures(material, DIFFUSE, "texture_diffuse");
		Textures.insert(Textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<MeshData::Texture> specularMaps = LoadMaterialTextures(material, SPECULAR, "texture_specular");
		Textures.insert(Textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<MeshData::Texture> normalMaps = LoadMaterialTextures(material, HEIGHT, "texture_normal");
		Textures.insert(Textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<MeshData::Texture> heightMaps = LoadMaterialTextures(material, AMBIENT, "texture_height");
		Textures.insert(Textures.end(), heightMaps.begin(), heightMaps.end());

		unsigned int MaterialID = LoadMaterialParam(material);

		// return a mesh object created from the extracted mesh data
		return MeshData(Vertices, Indices, Textures, MaterialID);
	}

	std::vector<MeshData::Texture> Model::LoadMaterialTextures(aiMaterial* Mat, TextureType Type, std::string TypeName)
	{
		std::vector<MeshData::Texture> textures;
		for (unsigned int i = 0; i < Mat->GetTextureCount(static_cast<aiTextureType>(Type)); i++)
		{
			aiString str;
			Mat->GetTexture(static_cast<aiTextureType>(Type), i, &str);
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
		MaterialData Data(Vector3(Ambient.r, Ambient.g, Ambient.b), Vector3(Diffuse.r, Diffuse.g, Diffuse.b), Vector3(Specular.r, Specular.g, Specular.b), Shininess);
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
			else
			{
				RZ_CORE_WARN("Model::TextureFromFile -> Path was {0} and was able to load file but couldn't handle number of components for color defaulting to just red channel", Path);
				format = GL_RED;
			}

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