#include "SceneSerializer.h"
#include <fstream>
#include "../Coordinator.h"
#include "../Component.h"

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
}

namespace Razor
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

	void SceneSerializer::SerializeEntity(YAML::Emitter& Out, Entity InEntity)
	{
		Out << YAML::BeginMap;
		Out << YAML::Key << "Entity" << YAML::Value << (uint32_t)InEntity.EntityHandle;
		if (InEntity.HasComponent<Transform>())
		{
			Out << YAML::Key << "Transform";
			Out << YAML::BeginMap;

			Transform& EntityTransform = InEntity.GetComponent<Transform>();
			Out << YAML::Key << "Position" << YAML::Value << EntityTransform.Position;
			Out << YAML::Key << "Rotation" << YAML::Value << EntityTransform.Rotation;
			Out << YAML::Key << "Scale" << YAML::Value << EntityTransform.Scale;
			Out << YAML::EndMap;
		}
		//if (InEntity.HasComponent<Mesh>())
		//{
		//	Out << YAML::Key << "Mesh";
		//	Out << YAML::BeginMap;

		//	Mesh& EntityMesh = InEntity.GetComponent<Mesh>();
		//	Out << YAML::Key << "Data" << YAML::Value << YAML::BeginSeq;
		//	//Out << YAML::Flow << EntityMesh.Data;
		//	Out << YAML::EndSeq;
		//	Out << YAML::EndMap;
		//}

		Out << YAML::EndMap;
	}



	void SceneSerializer::Serialize(Ref<Scene> OutScene)
	{
		YAML::Emitter Out;
		Out << YAML::BeginMap;
		Out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		Out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		auto View = OutScene->GetEntitiesWithComponents<Transform>();
		for (auto Handle : View)
		{
			SerializeEntity(Out, *OutScene->GetEntity(Handle));
		}
		Out << YAML::EndSeq;
		Out << YAML::EndMap;

		std::ofstream FOut(OutScene->GetPath().c_str());
		const char* ErrorMsg = new char(' ');
		std::perror(ErrorMsg);
		RZ_CORE_WARN("Error Msg: {0}", ErrorMsg);
		FOut << Out.c_str();
		FOut.close();
	}

	void SceneSerializer::Deserialize(Ref<Scene> OutScene)
	{
		YAML::Node Data;
		try
		{
			Data = YAML::LoadFile(OutScene->GetPath());
		}
		catch (YAML::ParserException e)
		{
			RZ_CORE_ERROR("Failed to load .rzscn file '{0}'\n	{1}", OutScene->GetPath(), e.what());
			return;
		}

		if (!Data["Scene"])
		{
			RZ_CORE_ERROR("Incomplete .rzscn file missing 'Scene' key");
			return;
		}

		std::string SceneName = Data["Scene"].as<std::string>();
		RZ_CORE_TRACE("Deserializing scene '{0}'", SceneName);

		auto Entities = Data["Entities"];
		if (Entities)
		{
			for (auto EntityNode : Entities)
			{
				Ref<Entity> DeserializedEntity = OutScene->CreateEntity();
				auto TransformComponent = EntityNode["Transform"];
				if (TransformComponent)
				{
					glm::vec3 Position = TransformComponent["Position"].as<glm::vec3>();
					glm::vec3 Rotation = TransformComponent["Rotation"].as<glm::vec3>();
					glm::vec3 Scale = TransformComponent["Scale"].as<glm::vec3>();
					Transform EntityTransform = { Position, Scale, Rotation };
					DeserializedEntity->AddComponent<Transform>(EntityTransform);
				}
				auto MeshComponent = EntityNode["Mesh"];
				/*if (MeshComponent)
				{
					std::vector<MeshData> Data = MeshComponent["Data"].as<std::vector<MeshData>>();
					Mesh EntityMesh = { Data };
					DeserializedEntity->AddComponent<Mesh>(EntityMesh);
				}*/
			}
		}
	}
}