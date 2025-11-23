#include "Scene.h"
#include "../Core/Entity.h"
#include "../Component.h"
#include <string>
#include "../Engine.h"
#include "../Scripting/ScriptInterface.h"
#include "../Scripting/ScriptEngine.h"
#include "../Log.h"
#include "../Utils/Vector.h"

namespace Razor
{
	Scene::Scene(const std::string& Path)
	{
		FilePath = Path;
	}
	Scene::~Scene()
	{

	}

	Ref<Entity> Scene::CreateEntity()
	{
		auto entityHandle = registry.create();
		Ref<Entity> ent = CreateRef<Entity>(entityHandle, this);
		ent->AddComponent<Transform>();
		ent->AddComponent<ScriptComponent>();
		return ent;
	}
	
	Ref<Entity> Scene::GetEntity(entt::entity EntityHandle)
	{
		Ref<Entity> Entt = CreateRef<Entity>(EntityHandle, this);
		
		return Entt->HasComponent<Transform>() ? Entt : nullptr;
	}
	
	void Scene::RunSystems(float DeltaTime)
	{
		for (uint64_t handle : mSystemInstanceHandles)
		{
			ScriptInterface& interface = Engine::Get().GetScriptInterface();
			// TODO do we actually need ScriptObject does it serve any purpose?
			ScriptObject obj = ScriptObject{ static_cast<int>(interface.GetScriptInstance(handle).handle) };
			Engine::Get().GetScriptInterface().InvokeMethod(obj, "Run", DeltaTime);
		}
	}

	void Scene::CreateSystemInstance(const Razor::ScriptClass& Type)
	{
		RZ_CORE_INFO("Instantiating system type {0}", Type.GetName());
		uint64_t handle = Engine::Get().GetScriptInterface().CreateScriptInstance(Type);
		mSystemInstanceHandles.push_back(handle);
	}

	std::vector<std::string> Scene::GetSystemNames() const
	{
		std::vector<std::string> Names(mSystemInstanceHandles.size());
		for (uint64_t handle : mSystemInstanceHandles)
		{
			ScriptInterface& interface = Engine::Get().GetScriptInterface();
			Names.push_back(interface.GetScriptInstance(handle).className);
		}
		return Names;
	}

	void Scene::StartScene()
	{
		CreateInstanceObjects(mSystemInstanceHandles);

		for (auto entity : GetEntitiesWithComponents<ScriptComponent>())
		{
			CreateInstanceObjects(GetEntity(entity)->GetComponent<ScriptComponent>().mScriptInstances);
		}
	}

	void Scene::CreateInstanceObjects(std::vector<uint64_t> handles)
	{
		ScriptInterface& interface = Engine::Get().GetScriptInterface();
		for (auto handle : handles)
		{
			ScriptInstance& instance = interface.GetScriptInstance(handle);
			ScriptObject obj = interface.CreateInstance(interface.GetType(instance.className));
			if (!obj)
			{
				RZ_CORE_ERROR("Scene(CreateInstanceObjects): -> Script Object ptr was invalid for instance of type: {0}", instance.className);
				continue;
			}
			PopulateObjectFields(instance, obj.id);
			instance.handle = obj.id;
		}
	}

	void Scene::PopulateObjectFields(ScriptInstance& instance, uint64_t objId)
	{
		ScriptInterface& interface = Engine::Get().GetScriptInterface();
		Ref<Coral::ManagedObject> manObj = interface.GetManagedObject(objId);
		if (!manObj)
		{
			RZ_CORE_ERROR("Scene(PopulateObjectFields): -> Managed Object ptr was null for id: {0}", objId);
			return;
		}
		for (auto field : instance.fields)
		{
			switch (field.Field.GetType())
			{
			case ScriptFieldType::Float:
			{
				float value = field.GetValue<float>();
				manObj->SetFieldValue<float>(field.Field.Name, value);
				break;
			}
			case ScriptFieldType::Double:
			{
				double value = field.GetValue<double>();
				manObj->SetFieldValue<double>(field.Field.Name, value);
				break;
			}
			case ScriptFieldType::Bool:
			{
				bool value = field.GetValue<bool>();
				manObj->SetFieldValue<bool>(field.Field.Name, value);
				break;
			}
			case ScriptFieldType::Char:
			{
				char value = field.GetValue<char>();
				manObj->SetFieldValue<char>(field.Field.Name, value);
				break;
			}
			case ScriptFieldType::String:
			{
				std::string value = field.GetValue<std::string>();
				Coral::String valueStr = Coral::String::New(value);
				manObj->SetFieldValue<Coral::String>(field.Field.Name, valueStr);
				break;
			}
			case ScriptFieldType::Int:
			{
				int value = field.GetValue<int>();
				manObj->SetFieldValue<int>(field.Field.Name, value);
				break;
			}
			case ScriptFieldType::Vector2:
			{
				Vector2 value = field.GetValue<Vector2>();
				manObj->SetFieldValue<Vector2>(field.Field.Name, value);
				break;
			}
			case ScriptFieldType::Vector3:
			{
				Vector3 value = field.GetValue<Vector3>();
				manObj->SetFieldValue<Vector3>(field.Field.Name, value);
				break;
			}
			case ScriptFieldType::Entity:
			{
				Entity value = field.GetValue<Entity>();
				manObj->SetFieldValue<Entity>(field.Field.Name, value);
				break;
			}
			}
		}
	}

	void Scene::StopScene()
	{
		ScriptInterface& interface = Engine::Get().GetScriptInterface();
		for (auto handle : mSystemInstanceHandles)
		{
			interface.DestroyInstanceObject(handle);
		}

		for (auto entity : GetEntitiesWithComponents<ScriptComponent>())
		{
			for (uint64_t handle : GetEntity(entity)->GetComponent<ScriptComponent>().mScriptInstances)
			{
				interface.DestroyInstanceObject(handle);
			}
		}

		interface.ClearObjectPool();
	}
}