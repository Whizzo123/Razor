#include "SceneSerializer.h"
#include <fstream>
#include "../Coordinator.h"
#include "../Component.h"
#include "../IO/YamlWrapper.h"
#include "../Utils/Vector.h"

namespace Razor
{
	Vector3 ToVec3(const glm::vec3& Vec)
	{
		return Vector3(Vec.x, Vec.y, Vec.z);
	}

	glm::vec3 ToGVec3(const Vector3& Vec)
	{
		return glm::vec3(Vec.X, Vec.Y, Vec.Z);
	}

	void SceneSerializer::SerializeEntity(YamlEmitter* Out, Entity InEntity)
	{
		yaml_emitter_begin_map(Out);
		yaml_emitter_key(Out, "Entity");
		yaml_emitter_value_int32(Out, (uint32_t)InEntity.EntityHandle);
		if (InEntity.HasComponent<Transform>())
		{
			yaml_emitter_key(Out, "Transform");
			yaml_emitter_begin_map(Out);

			Transform& EntityTransform = InEntity.GetComponent<Transform>();
			yaml_emitter_key(Out, "Position");
			yaml_emitter_value_vec3(Out, ToVec3(EntityTransform.Position));
			yaml_emitter_key(Out, "Rotation");
			yaml_emitter_value_vec3(Out, ToVec3(EntityTransform.Rotation));
			yaml_emitter_key(Out, "Scale");
			yaml_emitter_value_vec3(Out, ToVec3(EntityTransform.Scale));
			yaml_emitter_end_map(Out);
			
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

		yaml_emitter_end_map(Out);
	}



	void SceneSerializer::Serialize(Ref<Scene> OutScene)
	{
		YamlEmitter* Out = yaml_emitter_new();
		yaml_emitter_begin_map(Out);
		yaml_emitter_key(Out, "Scene");
		yaml_emitter_value_string(Out, "Untitled");
		yaml_emitter_key(Out, "Entities");
		yaml_emitter_value_seq(Out);
		auto View = OutScene->GetEntitiesWithComponents<Transform>();
		for (auto Handle : View)
		{
			SerializeEntity(Out, *OutScene->GetEntity(Handle));
		}
		yaml_emitter_end_seq(Out);
		yaml_emitter_end_map(Out);

		std::ofstream FOut(OutScene->GetPath().c_str());
		const char* ErrorMsg = new char(' ');
		std::perror(ErrorMsg);
		RZ_CORE_WARN("Error Msg: {0}", ErrorMsg);
		FOut << yaml_emitter_cstr(Out);
		FOut.close();
	}

	bool SceneSerializer::Deserialize(Ref<Scene> OutScene)
	{
		bool bContainsExt = false;
		for (int i = 0; i < OutScene->GetPath().length(); i++)
		{
			if (OutScene->GetPath()[i] == '.')
			{
				const std::string Extension = OutScene->GetPath().substr(i, 6);
				if (Extension == ".rzscn")
				{
					bContainsExt = true;
					break;
				}
			}
		}
		if (bContainsExt == false)
		{
			return false;
		}
		struct stat Buffer;
		if (stat(OutScene->GetPath().c_str(), &Buffer) != 0)
		{
			RZ_CORE_WARN("SceneSerializer.Deserialize: Attempted to load scene file that didn't exist called: {0}", OutScene->GetPath());
			return false;
		}
		YamlNode* Data = yaml_load_file(OutScene->GetPath().c_str());
		if(!Data)
		{
			RZ_CORE_ERROR("Failed to load .rzscn file '{0}'\n	{1}", OutScene->GetPath(), yaml_get_last_error());
			return false;
		}

		if (!yaml_get_child(Data, "Scene"))
		{
			RZ_CORE_ERROR("Incomplete .rzscn file missing 'Scene' key");
			return false;
		}

		std::string SceneName = yaml_as_string(yaml_get_child(Data, "Scene"));
		RZ_CORE_TRACE("Deserializing scene '{0}'", SceneName);

		auto Entities = yaml_get_child(Data, "Entities");
		if (Entities)
		{
			for (auto EntityNode : yaml_get_children(Data, "Entities"))
			{
				Ref<Entity> DeserializedEntity = OutScene->CreateEntity();
				auto TransformComponent = yaml_get_child(Data, "Transform");
				if (TransformComponent)
				{
					glm::vec3 Position = ToGVec3(yaml_as_vec3(yaml_get_child(TransformComponent, "Position")));
					glm::vec3 Rotation = ToGVec3(yaml_as_vec3(yaml_get_child(TransformComponent, "Rotation")));
					glm::vec3 Scale = ToGVec3(yaml_as_vec3(yaml_get_child(TransformComponent, "Scale")));
					Transform EntityTransform = { Position, Scale, Rotation };
					DeserializedEntity->AddComponent<Transform>(EntityTransform);
				}
				//auto MeshComponent = EntityNode["Mesh"];
				/*if (MeshComponent)
				{
					std::vector<MeshData> Data = MeshComponent["Data"].as<std::vector<MeshData>>();
					Mesh EntityMesh = { Data };
					DeserializedEntity->AddComponent<Mesh>(EntityMesh);
				}*/
			}
		}
		return true;
	}
}