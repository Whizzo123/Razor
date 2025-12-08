#include "ComponentImGui.h"
#include "AssetPickerPopupWindow.h"

namespace EdgeEditor
{
	std::unordered_map<std::string, ComponentDrawer> ComponentImGui::_ComponentDrawers = RegisterDrawers();

	Razor::Ref<AssetPickerPopupWindow> ComponentImGui::_mAssetPickerPopup = nullptr;

	std::unordered_map<std::string, ComponentDrawer> ComponentImGui::RegisterDrawers()
	{
		std::unordered_map<std::string, ComponentDrawer> map = std::unordered_map<std::string, ComponentDrawer>();
		map["Transform"] = DrawTransform;
		map["Mesh"] = DrawMesh;
		map["DirectionalLight"] = DrawDirectionalLight;
		map["ScriptComponent"] = DrawScriptComponent;
		map["BoxBody"] = DrawBoxBody;
		return map;
	}

	void ComponentImGui::DrawComponents(Razor::Ref<Razor::Entity> InEntity)
	{
		for (auto& [name, drawer] : _ComponentDrawers)
		{
			drawer(InEntity);
		}
	}

	void ComponentImGui::SetAssetPickerPopup(Razor::Ref<AssetPickerPopupWindow> InPopup)
	{
		_mAssetPickerPopup = InPopup;
	}

	void ComponentImGui::DrawTransform(Razor::Ref<Razor::Entity> InEntity)
	{
		if (InEntity->HasComponent<Razor::Transform>())
		{
			Razor::Transform& transform = InEntity->GetComponent<Razor::Transform>();
			if (Razor::RazorImGui::CollapsingHeader("Transform"))
			{
				if (Razor::RazorImGui::CollapsingHeader("Position"))
				{
					Razor::RazorImGui::InputFloat("X", &transform.Position.x);
					Razor::RazorImGui::InputFloat("Y", &transform.Position.y);
					Razor::RazorImGui::InputFloat("Z", &transform.Position.z);
				}
			}
		}
	}

	void ComponentImGui::DrawMesh(Razor::Ref<Razor::Entity> InEntity)
	{
		if (InEntity->HasComponent<Razor::Mesh>())
		{
			Razor::Mesh& mesh = InEntity->GetComponent<Razor::Mesh>();
			if (Razor::RazorImGui::CollapsingHeader("Mesh"))
			{
				Razor::RazorImGui::Text(mesh.mKey.GetKey().c_str());
				if (Razor::RazorImGui::Button("Pick"))
				{
					if (_mAssetPickerPopup == nullptr)
					{
						_mAssetPickerPopup = Razor::CreateRef<AssetPickerPopupWindow>("C:/Sandbox/Razor/Sandbox/assets");
					}
				}
				if (_mAssetPickerPopup)
				{
					if (_mAssetPickerPopup->Draw() == false)
					{
						if (_mAssetPickerPopup->GetSelectedAssetPath() != mesh.mKey.GetKey())
						{
							mesh.mKey = Razor::AssetKey{ _mAssetPickerPopup->GetSelectedAssetPath()};
						}
						_mAssetPickerPopup = nullptr;
					}
				}
			}
		}
	}

	void ComponentImGui::DrawDirectionalLight(Razor::Ref<Razor::Entity> InEntity)
	{
		if (InEntity->HasComponent<Razor::DirectionalLight>())
		{
			Razor::DirectionalLight& mesh = InEntity->GetComponent<Razor::DirectionalLight>();
			if (Razor::RazorImGui::CollapsingHeader("Directional Light"))
			{
				// Nothing really here to show just now maybe in the future but at least we know it's there
			}
		}
	}

	void ComponentImGui::DrawScriptComponent(Razor::Ref<Razor::Entity> InEntity)
	{
		if (InEntity->HasComponent<Razor::ScriptComponent>())
		{
			Razor::ScriptComponent& scriptComp = InEntity->GetComponent<Razor::ScriptComponent>();
			for (uint64_t instanceID : scriptComp.mScriptInstances)
			{
				Razor::ScriptInstance& instance = Razor::Engine::Get().GetScriptInterface().GetScriptInstance(instanceID);
				if (Razor::RazorImGui::CollapsingHeader(instance.className.c_str()))
				{
					for (Razor::ScriptFieldInstance& field : instance.fields)
					{
						Razor::ScriptFieldType type = field.Field.GetType();
						if (type == Razor::ScriptFieldType::String)
						{
							std::string value = field.GetValue<std::string>();
							char buffer[256];
							strncpy(buffer, value.c_str(), sizeof(buffer));
							if (Razor::RazorImGui::InputText(field.Field.Name.c_str(), buffer, sizeof(buffer)))
							{
								field.SetValue<std::string>(std::string(buffer));
							}
						}
					}
				}
			}
			
		}
	}

	void ComponentImGui::DrawBoxBody(Razor::Ref<Razor::Entity> inEntity)
	{
		if (inEntity->HasComponent<Razor::BoxBody>())
		{
			Razor::BoxBody& body = inEntity->GetComponent<Razor::BoxBody>();
			if (Razor::RazorImGui::CollapsingHeader("Box Body"))
			{
				if (Razor::RazorImGui::CheckBox("Use Gravity", &body.mbUseGravity))
				{

				}
			}
		}
	}
}