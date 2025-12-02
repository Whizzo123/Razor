#include "SceneSerializer.h"
#include <fstream>
#include "../Coordinator.h"
#include "../Component.h"
#include "../IO/YamlWrapper.h"
#include "../Utils/Vector.h"
#include "../Core/Entity.h"
#include "Scene.h"
#include "../Scripting/ScriptEngine.h"
#include "../Scripting/ScriptInterface.h"
#include "../Assert.h"
#include "../Engine.h"

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
		if (InEntity.HasComponent<Mesh>())
		{
			yaml_emitter_key(Out, "Mesh");
			yaml_emitter_begin_map(Out);

			Mesh& EntityMesh = InEntity.GetComponent<Mesh>();
			yaml_emitter_key(Out, "ModelKey");
			yaml_emitter_value_string(Out, EntityMesh.mKey.GetKey().c_str());
			
			yaml_emitter_end_map(Out);
		}
		if (InEntity.HasComponent<DirectionalLight>())
		{
			yaml_emitter_key(Out, "DirectionalLight");
			yaml_emitter_begin_map(Out);

			DirectionalLight& DirLight = InEntity.GetComponent<DirectionalLight>();
			yaml_emitter_key(Out, "Diffuse");
			yaml_emitter_value_vec3(Out, ToVec3(DirLight.Diffuse));
			yaml_emitter_key(Out, "Ambient");
			yaml_emitter_value_vec3(Out, ToVec3(DirLight.Ambient));
			yaml_emitter_key(Out, "Specular");
			yaml_emitter_value_vec3(Out, ToVec3(DirLight.Specular));
			yaml_emitter_key(Out, "Direction");
			yaml_emitter_value_vec3(Out, ToVec3(DirLight.Direction));

			yaml_emitter_end_map(Out);
		}
		if (InEntity.HasComponent<ScriptComponent>())
		{
			auto& scriptComponent = InEntity.GetComponent<ScriptComponent>();

			yaml_emitter_key(Out, "ScriptComponent");
			yaml_emitter_value_seq(Out);

			// Fields
			for (uint64_t instanceID : scriptComponent.mScriptInstances)
			{
				ScriptInstance& instance = Engine::Get().GetScriptInterface().GetScriptInstance(instanceID);
				const auto& fields = instance.fields;
				// NOTE: You cannot use keys in a sequence it has to begin with a map
				yaml_emitter_begin_map(Out); // Script instance map entry
				yaml_emitter_key(Out, instance.className.c_str());
				yaml_emitter_begin_map(Out); // ScriptInstance
				if (fields.size() > 0)
				{
					yaml_emitter_key(Out, "ScriptFields");
					yaml_emitter_value_seq(Out); //Script Fields
					for (ScriptFieldInstance field : fields)
					{
						yaml_emitter_begin_map(Out); // Field instance map entry
						yaml_emitter_key(Out, field.Field.Name.c_str());
						yaml_emitter_begin_map(Out); //Field instance
						yaml_emitter_key(Out, "Type");
						yaml_emitter_value_string(Out, Utils::ScriptFieldTypeToString(field.Field.GetType()));

						yaml_emitter_key(Out, "Data");

						switch (field.Field.GetType())
						{
						case ScriptFieldType::Float:
							yaml_emitter_value_float(Out, field.GetValue<float>());
							break;
						case ScriptFieldType::Double:
							yaml_emitter_value_double(Out, field.GetValue<double>());
							break;
						case ScriptFieldType::Bool:
							yaml_emitter_value_bool(Out, field.GetValue<bool>());
							break;
						case ScriptFieldType::Char:
							yaml_emitter_value_char(Out, field.GetValue<char>());
							break;
						case ScriptFieldType::String:
							yaml_emitter_value_string(Out, field.GetValue<std::string>().c_str());
							break;
						case ScriptFieldType::Int:
							yaml_emitter_value_int(Out, field.GetValue<int>());
							break;
						case ScriptFieldType::Vector2:
							yaml_emitter_value_vec2(Out, field.GetValue<Vector2>());
							break;
						case ScriptFieldType::Vector3:
							yaml_emitter_value_vec3(Out, field.GetValue<Vector3>());
							break;
						case ScriptFieldType::Entity:
							yaml_emitter_value_int32(Out, field.GetValue<uint32_t>());
							break;
						}
						yaml_emitter_end_map(Out); // Field instance
						yaml_emitter_end_map(Out); // Field instance map entry 
					}
					yaml_emitter_end_seq(Out); // ScriptFields
				}
				yaml_emitter_end_map(Out); // ScriptInstance
				yaml_emitter_end_map(Out); // ScriptInstance map entry
			}

			yaml_emitter_end_seq(Out); // ScriptComponent
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
		yaml_emitter_key(Out, "Systems");
		yaml_emitter_value_seq(Out);
		for (uint64_t handle : OutScene->mSystemInstanceHandles)
		{
			ScriptInstance& instance = Engine::Get().GetScriptInterface().GetScriptInstance(handle);
			yaml_emitter_value_string(Out, instance.className.c_str());
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

		auto Systems = yaml_get_child(Data, "Systems");
		for (auto SystemNode : yaml_get_children(Data, "Systems"))
		{
			std::string typeName = yaml_as_string(SystemNode);
			Razor::ScriptClass type = Engine::Get().GetScriptInterface().GetType(typeName);
			if (type)
			{
				RZ_CORE_INFO("Creating System Object for type: {0}", typeName);
				OutScene->CreateSystemInstance(type);
			}
			else
			{
				RZ_CORE_WARN("Could not find type for system: {0}", typeName);
			}
		}
		return true;
	}

	void SceneSerializer::DeserializeEntity(YamlNode* EntityNode, Ref<Scene> OutScene)
	{
		Ref<Entity> DeserializedEntity = OutScene->CreateEntity();
		auto TransformComponent = yaml_get_child(EntityNode, "Transform");
		/* This is a one-off case as all entities are created with transform & script components so we don't need to add one just pass data */
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
			Razor::ScriptComponent& sc = DeserializedEntity->GetComponent<Razor::ScriptComponent>();
			ScriptInterface& interface = Engine::Get().GetScriptInterface();
			for (const auto& node : yaml_get_children(ScriptComponent)) // Seq
			{
				for (const auto& [className, classNode] : yaml_get_children_map(node)) // Map (each entry is single key class name)
				{
					uint64_t instanceId = interface.CreateScriptInstance(interface.GetType(className));
					ScriptInstance& instance = interface.GetScriptInstance(instanceId);

					auto scriptFieldsNode = yaml_get_child(classNode, "ScriptFields");
					for (const auto& fieldEntry : yaml_get_children(scriptFieldsNode)) // Seq
					{
						for (const auto& [fieldName, fieldNode] : yaml_get_children_map(fieldEntry)) // Map (each entry is name of field with mapped details)
						{
							std::string typeString = yaml_as_string(yaml_get_child(fieldNode, "Type"));
							ScriptFieldInstance& fieldInstance = instance.GetFieldInstance(fieldName);
							ScriptFieldType type = Utils::ScriptFieldTypeFromString(typeString);


							switch (type)
							{
							case ScriptFieldType::Float:
							{
								float data = yaml_as_float(yaml_get_child(fieldNode, "Data"), 0.0f);
								fieldInstance.SetValue(data);
								break;
							}
							case ScriptFieldType::Double:
							{
								double data = yaml_as_double(yaml_get_child(fieldNode, "Data"), 0.0);
								fieldInstance.SetValue(data);
								break;
							}
							case ScriptFieldType::Bool:
							{
								bool data = yaml_as_bool(yaml_get_child(fieldNode, "Data"), false);
								fieldInstance.SetValue(data);
								break;
							}
							case ScriptFieldType::Char:
							{
								char data = yaml_as_char(yaml_get_child(fieldNode, "Data"));
								fieldInstance.SetValue(data);
								break;
							}
							case ScriptFieldType::String:
							{
								std::string data = yaml_as_string(yaml_get_child(fieldNode, "Data"));
								fieldInstance.SetValue(data);
								break;
							}
							case ScriptFieldType::Int:
							{
								int data = yaml_as_int(yaml_get_child(fieldNode, "Data"), 0);
								fieldInstance.SetValue(data);
								break;
							}
							case ScriptFieldType::Vector2:
							{
								Vector2 data = yaml_as_vec2(yaml_get_child(fieldNode, "Data"));
								fieldInstance.SetValue(data);
								break;
							}
							case ScriptFieldType::Vector3:
							{
								Vector3 data = yaml_as_vec3(yaml_get_child(fieldNode, "Data"));
								fieldInstance.SetValue(data);
								break;
							}
							case ScriptFieldType::Entity:
							{
								uint32_t data = yaml_as_int32(yaml_get_child(fieldNode, "Data"), 0);
								fieldInstance.SetValue(data);
								break;
							}
							}
						}
					}
					sc.mScriptInstances.push_back(instanceId);
				}
			}
		}
		auto MeshComponent = yaml_get_child(EntityNode, "Mesh");
		if (MeshComponent)
		{
			AssetKey key(yaml_as_string(yaml_get_child(MeshComponent, "ModelKey")));
			Mesh EntityMesh = { key };
			DeserializedEntity->AddComponent<Mesh>(EntityMesh);
		}
		auto DirectionalLightComponent = yaml_get_child(EntityNode, "DirectionalLight");
		if(DirectionalLightComponent)
		{
			DirectionalLight dirLight;
			dirLight.Diffuse = ToGVec3(yaml_as_vec3(yaml_get_child(DirectionalLightComponent, "Diffuse")));
			dirLight.Ambient = ToGVec3(yaml_as_vec3(yaml_get_child(DirectionalLightComponent, "Ambient")));
			dirLight.Specular = ToGVec3(yaml_as_vec3(yaml_get_child(DirectionalLightComponent, "Specular")));
			dirLight.Direction = ToGVec3(yaml_as_vec3(yaml_get_child(DirectionalLightComponent, "Direction")));
			DeserializedEntity->AddComponent<DirectionalLight>(dirLight);
		}
	}
}