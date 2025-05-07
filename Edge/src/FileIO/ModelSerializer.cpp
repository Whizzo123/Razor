#include "ModelSerializer.h"
#include <fstream>

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Razor::MeshData::Vertex>
	{
		static Node encode(const Razor::MeshData::Vertex& rhs)
		{
			Node node;
			node.push_back(rhs.Position);
			node.push_back(rhs.Normal);
			node.push_back(rhs.TexCoords);
			node.push_back(rhs.Tangent);
			node.push_back(rhs.Bitangent);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, Razor::MeshData::Vertex& rhs)
		{
			if (!node.IsSequence() || node.size() != 5)
				return false;

			rhs.Position = node[0].as<glm::vec3>();
			rhs.Normal = node[1].as<glm::vec3>();
			rhs.TexCoords = node[2].as<glm::vec2>();
			rhs.Tangent = node[3].as<glm::vec3>();
			rhs.Bitangent = node[4].as<glm::vec3>();
			return true;
		}
	};

	template<>
	struct convert<Razor::MeshData::Texture>
	{
		static Node encode(const Razor::MeshData::Texture& rhs)
		{
			Node node;
			node.push_back(rhs.Id);
			node.push_back(rhs.Type);
			node.push_back(rhs.Path);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, Razor::MeshData::Texture& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.Id = node[0].as<unsigned int>();
			rhs.Type = node[1].as<std::string>();
			rhs.Path = node[2].as<std::string>();
			return true;
		}
	};

	template<>
	struct convert<Razor::MeshData>
	{
		static Node encode(const Razor::MeshData& rhs)
		{
			Node node;
			node.push_back(rhs.Vertices);
			node.push_back(rhs.Meshes);
			node.push_back(rhs.Indices);
			node.push_back(rhs.Textures);
			node.push_back(rhs.VAO);
			node.push_back(rhs.VBO);
			node.push_back(rhs.EBO);
			node.push_back(rhs.MaterialId);
			node.push_back(rhs.bHasIndices);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, Razor::MeshData& rhs)
		{
			if (!node.IsSequence() || node.size() != 9)
				return false;

			rhs.Vertices = node[0].as<std::vector<Razor::MeshData::Vertex>>();
			rhs.Meshes = node[1].as<std::vector<Razor::MeshData>>();
			rhs.Indices = node[2].as<std::vector<unsigned int>>();
			rhs.Textures = node[3].as<std::vector<Razor::MeshData::Texture>>();
			rhs.VAO = node[4].as<unsigned int>();
			rhs.VBO = node[5].as<unsigned int>();
			rhs.EBO = node[6].as<unsigned int>();
			rhs.MaterialId = node[7].as<unsigned int>();
			rhs.bHasIndices = node[8].as<bool>();
			return true;
		}
	};

	template<>
	struct convert<Razor::MaterialData>
	{
		static Node encode(const Razor::MaterialData& rhs)
		{
			Node node;
			node.push_back(rhs.MaterialName);
			node.push_back(rhs.Ambient);
			node.push_back(rhs.Diffuse);
			node.push_back(rhs.Specular);
			node.push_back(rhs.Shininess);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, Razor::MaterialData& rhs)
		{
			if (!node.IsSequence() || node.size() != 5)
				return false;

			rhs.MaterialName = node[0].as<std::string>();
			rhs.Ambient = node[1].as<glm::vec3>();
			rhs.Diffuse = node[2].as<glm::vec3>();
			rhs.Specular = node[3].as<glm::vec3>();
			rhs.Shininess = node[4].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Razor::Material>
	{
		static Node encode(const Razor::Material& rhs)
		{
			Node node;
			node.push_back(rhs.Materials);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, Razor::Material& rhs)
		{
			if (!node.IsSequence() || node.size() != 1)
				return false;

			rhs.Materials = node[0].as<std::vector<Razor::MaterialData>>();
			return true;
		}
	};

	template<>
	struct convert<Razor::ModelInfo>
	{
		static Node encode(const Razor::ModelInfo& rhs)
		{
			Node node;
			node.push_back(rhs.ModelMeshData);
			node.push_back(rhs.ModelMaterial);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, Razor::ModelInfo& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.ModelMeshData = node[0].as<std::vector<Razor::MeshData>>();
			rhs.ModelMaterial = node[1].as<Razor::Material>();
			return true;
		}
	};
}

namespace EdgeEditor
{
	YAML::Emitter& operator<<(YAML::Emitter& Out, const glm::vec3& Vec)
	{
		Out << YAML::Flow;
		Out << YAML::BeginSeq << Vec.x << Vec.y << Vec.z << YAML::EndSeq;
		return Out;
	}

	template<typename T>
	YAML::Emitter& operator<<(YAML::Emitter& Out, const T& rhs)
	{
		Out << YAML::convert<T>::encode(rhs);
		return Out;
	}

	void ModelSerializer::Serialize(const std::string& Path, Razor::Ref<Razor::Model> Model)
	{
		YAML::Emitter Out;
		Out << YAML::BeginMap;
		Out << YAML::Key << "ModelInfo" << YAML::Value << Model->GetModelInfo();
		Out << YAML::EndMap;

		const std::string PathPlusExt = Path + ".model";

		std::ofstream FOut(PathPlusExt.c_str());
		const char* ErrorMsg = new char(' ');
		std::perror(ErrorMsg);
		RZ_WARN("Error Msg: {0}", ErrorMsg);
		FOut << Out.c_str();
		FOut.close();
	}

	void ModelSerializer::Deserialize(const std::string& Path, Razor::Ref<Razor::Model> OutModel)
	{
		YAML::Node Data;
		const std::string PathPlusExt = Path + ".model";
		try
		{
			Data = YAML::LoadFile(PathPlusExt);
		}
		catch (YAML::ParserException e)
		{
			RZ_ERROR("Failed to load .model file '{0}'\n	{1}", PathPlusExt, e.what());
			return;
		}

		if (!Data["ModelInfo"])
		{
			RZ_ERROR("Incomplete .model file missing 'ModelInfo' key");
			return;
		}

		Razor::ModelInfo Info = Data["ModelInfo"].as<Razor::ModelInfo>();
		OutModel->SetModelInfo(Info);
	}
}