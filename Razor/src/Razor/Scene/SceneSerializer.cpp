#include "SceneSerializer.h"
#include <fstream>
#include "../Coordinator.h"
#include "../Component.h"
#include "../IO/YamlWrapper.h"
#include "../Utils/Vector.h"
#include "../Core/Entity.h"
#include "Scene.h"
#include "../Scripting/ScriptEngine.h"
#include "../Assert.h"

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
		if (InEntity.HasComponent<ScriptComponent>())
		{
			auto& scriptComponent = InEntity.GetComponent<ScriptComponent>();

			yaml_emitter_key(Out, "ScriptComponent");
			yaml_emitter_begin_map(Out); // ScriptComponent
			yaml_emitter_key(Out, "ClassName");
			yaml_emitter_value_string(Out, scriptComponent.ClassName.c_str());

			// Fields
			Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(scriptComponent.ClassName);
			const auto& fields = entityClass->GetFields();
			if (fields.size() > 0)
			{
				yaml_emitter_key(Out, "ScriptFields");
				yaml_emitter_value_string(Out, "");
				yaml_emitter_begin_seq(Out);
				auto& entityFields = ScriptEngine::GetScriptFieldMap(InEntity);
				for (const auto& [name, field] : fields)
				{
					if (entityFields.find(name) == entityFields.end())
						continue;

					yaml_emitter_begin_map(Out); // ScriptField
					yaml_emitter_key(Out, "Name");
					yaml_emitter_value_string(Out, name.c_str());
					yaml_emitter_key(Out, "Type");
					yaml_emitter_value_string(Out, Utils::ScriptFieldTypeToString(field.GetType()));

					yaml_emitter_key(Out, "Data");
					yaml_emitter_value_string(Out, "");
					ScriptFieldInstance& scriptField = entityFields.at(name);

					switch (field.GetType())
					{
					case ScriptFieldType::Float:
						yaml_emitter_value_float(Out, scriptField.GetValue<float>());
						break;
					case ScriptFieldType::Double:
						yaml_emitter_value_double(Out, scriptField.GetValue<double>());
						break;
					case ScriptFieldType::Bool:
						yaml_emitter_value_bool(Out, scriptField.GetValue<bool>());
						break;
					case ScriptFieldType::Char:
						yaml_emitter_value_char(Out, scriptField.GetValue<char>());
						break;
					case ScriptFieldType::Int:
						yaml_emitter_value_int(Out, scriptField.GetValue<int>());
						break;
					case ScriptFieldType::Vector2:
						yaml_emitter_value_vec2(Out, scriptField.GetValue<Vector2>());
						break;
					case ScriptFieldType::Vector3:
						yaml_emitter_value_vec3(Out, scriptField.GetValue<Vector3>());
						break;
					case ScriptFieldType::Entity:
						yaml_emitter_value_int32(Out, scriptField.GetValue<uint32_t>());
						break;
					}
					yaml_emitter_end_map(Out); // ScriptFields
				}
				yaml_emitter_end_seq(Out);
			}

			yaml_emitter_end_map(Out); // ScriptComponent
		}

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
				DeserializeEntity(EntityNode, OutScene);
			}
		}
		return true;
	}

	void SceneSerializer::DeserializeEntity(YamlNode* EntityNode, Ref<Scene> OutScene)
	{
		Ref<Entity> DeserializedEntity = OutScene->CreateEntity();
		auto TransformComponent = yaml_get_child(EntityNode, "Transform");
		/* This is a one-off case as all entities are created with transform components so we don't need to add one just pass data */
		if (TransformComponent)
		{
			Transform& comp = DeserializedEntity->GetComponent<Transform>();
			comp.Position = ToGVec3(yaml_as_vec3(yaml_get_child(TransformComponent, "Position")));
			comp.Rotation = ToGVec3(yaml_as_vec3(yaml_get_child(TransformComponent, "Rotation")));
			comp.Scale = ToGVec3(yaml_as_vec3(yaml_get_child(TransformComponent, "Scale")));
		}
		auto ScriptComponent = yaml_get_child(EntityNode, "ScriptComponent");
		if (ScriptComponent)
		{
			Razor::ScriptComponent& sc = DeserializedEntity->AddComponent<Razor::ScriptComponent>();
			sc.ClassName = yaml_as_string(yaml_get_child(ScriptComponent, "ClassName"));
			YamlNode* Fields = yaml_get_child(ScriptComponent, "ScriptFields");
			if (Fields)
			{
				Ref<ScriptClass> componentClass = ScriptEngine::GetEntityClass(sc.ClassName);
				if (componentClass)
				{
					const auto& classFields = componentClass->GetFields();
					ScriptFieldMap& entityFields = ScriptEngine::GetScriptFieldMap(*DeserializedEntity);

					for (YamlNode* scriptField : yaml_get_children(ScriptComponent, "ScriptFields"))
					{
						std::string name = yaml_as_string(yaml_get_child(scriptField, "Name"));
						std::string typeString = yaml_as_string(yaml_get_child(scriptField, "Type"));
						ScriptFieldType type = Utils::ScriptFieldTypeFromString(typeString);

						ScriptFieldInstance& fieldInstance = entityFields[name];

						// TODO(Yan): turn this assert into Hazelnut log warning
						RZ_CORE_ASSERT(classFields.find(name) != classFields.end());

						if (classFields.find(name) == classFields.end())
							continue;

						fieldInstance.Field = classFields.at(name);

						switch (type)
						{
						case ScriptFieldType::Float:                   
							{                                                  
								float data = yaml_as_float(yaml_get_child(scriptField, "Data"), 0.0f);    
								fieldInstance.SetValue(data);                  
								break;                                         
							}
						case ScriptFieldType::Double:
						{
							double data = yaml_as_double(yaml_get_child(scriptField, "Data"), 0.0);
							fieldInstance.SetValue(data);
							break;
						}
						case ScriptFieldType::Bool:
						{
							bool data = yaml_as_bool(yaml_get_child(scriptField, "Data"), false);
							fieldInstance.SetValue(data);
							break;
						}
						case ScriptFieldType::Char:
						{
							char data = yaml_as_char(yaml_get_child(scriptField, "Data"));
							fieldInstance.SetValue(data);
							break;
						}
						case ScriptFieldType::Int:
						{
							int data = yaml_as_int(yaml_get_child(scriptField, "Data"), 0);
							fieldInstance.SetValue(data);
							break;
						}
						case ScriptFieldType::Vector2:
						{
							Vector2 data = yaml_as_vec2(yaml_get_child(scriptField, "Data"));
							fieldInstance.SetValue(data);
							break;
						}
						case ScriptFieldType::Vector3:
						{
							Vector3 data = yaml_as_vec3(yaml_get_child(scriptField, "Data"));
							fieldInstance.SetValue(data);
							break;
						}
						case ScriptFieldType::Entity:
						{
							uint32_t data = yaml_as_int32(yaml_get_child(scriptField, "Data"), 0);
							fieldInstance.SetValue(data);
							break;
						}
						}
					}
				}
			}
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