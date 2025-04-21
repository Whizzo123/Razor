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
}

namespace Razor
{
	YAML::Emitter& operator<<(YAML::Emitter& Out, const glm::vec3& Vec)
	{
		Out << YAML::Flow;
		Out << YAML::BeginSeq << Vec.x << Vec.y << Vec.z << YAML::EndSeq;
		return Out;
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter& Out, Entity InEntity)
	{
		/*Out << YAML::BeginMap;
		Out << YAML::Key << "Entity" << YAML::Value << InEntity;
		if (Coordinator::GetInstance()->HasComponent<Transform>(InEntity))
		{
			Out << YAML::Key << "Transform";
			Out << YAML::BeginMap;

			Transform& EntityTransform = Coordinator::GetInstance()->GetComponent<Transform>(InEntity);
			Out << YAML::Key << "Position" << YAML::Value << EntityTransform.Position;
			Out << YAML::Key << "Rotation" << YAML::Value << EntityTransform.Rotation;
			Out << YAML::Key << "Scale" << YAML::Value << EntityTransform.Scale;
			Out << YAML::EndMap;
		}

		Out << YAML::EndMap;*/
	}



	void SceneSerializer::Serialize(Ref<Scene> OutScene)
	{
		/*YAML::Emitter Out;
		Out << YAML::BeginMap;
		Out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		Out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		uint32_t CurrentEntityCount = Coordinator::GetInstance()->GetCurrentEntityCount();
		for (Entity e = 0; e < CurrentEntityCount; e++)
		{
			SerializeEntity(Out, e);
		}
		Out << YAML::EndSeq;
		Out << YAML::EndMap;

		std::ofstream FOut(OutScene->GetPath().c_str());
		const char* ErrorMsg = new char(' ');
		std::perror(ErrorMsg);
		RZ_CORE_WARN("Error Msg: {0}", ErrorMsg);
		FOut << Out.c_str();
		FOut.close();*/
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
			}
		}
	}
}