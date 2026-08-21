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
		map["Camera"] = DrawCamera;
		map["CollisionComponent"] = DrawCollisionComponent;
		map["Text"] = DrawTextComponent;
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
					Razor::RazorImGui::InputFloat("Pos X", &transform.Position.x);
					Razor::RazorImGui::InputFloat("Pos Y", &transform.Position.y);
					Razor::RazorImGui::InputFloat("Pos Z", &transform.Position.z);
				}
				if (Razor::RazorImGui::CollapsingHeader("Scale"))
				{
					Razor::RazorImGui::InputFloat("Sca X", &transform.Scale.x);
					Razor::RazorImGui::InputFloat("Sca Y", &transform.Scale.y);
					Razor::RazorImGui::InputFloat("Sca Z", &transform.Scale.z);
				}
				if (Razor::RazorImGui::CollapsingHeader("Rotation"))
				{
					Razor::RazorImGui::InputFloat("Rot X", &transform.Rotation.x);
					Razor::RazorImGui::InputFloat("Rot Y", &transform.Rotation.y);
					Razor::RazorImGui::InputFloat("Rot Z", &transform.Rotation.z);
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
				if(Razor::RazorImGui::Button("Remove Component", {200.0f, 30.0f}))
				{
					InEntity->RemoveComponent<Razor::Mesh>();
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
				if(Razor::RazorImGui::Button("Remove Component", {200.0f, 30.0f}))
				{
					InEntity->RemoveComponent<Razor::DirectionalLight>();
				}
			}
		}
	}

	void ComponentImGui::DrawScriptComponent(Razor::Ref<Razor::Entity> InEntity)
	{
		if (InEntity->HasComponent<Razor::ScriptComponent>())
		{
			Razor::ScriptComponent& scriptComp = InEntity->GetComponent<Razor::ScriptComponent>();
			std::vector<int> instancesToRemove;
			int i = 0;
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
						else if (type == Razor::ScriptFieldType::Int)
						{
							Razor::Ref<int> value = Razor::CreateRef<int>(field.GetValue<int>());
							if (Razor::RazorImGui::InputInt(field.Field.Name.c_str(), value))
							{
								field.SetValue<int>(*value);
							}
						}
						else if (type == Razor::ScriptFieldType::Float)
						{
							Razor::Ref<float> value = Razor::CreateRef<float>(field.GetValue<float>());
							if (Razor::RazorImGui::InputFloat(field.Field.Name.c_str(), value.get()))
							{
								field.SetValue<float>(*value);
							}
						}
					}
					if(Razor::RazorImGui::Button("Remove Component", {200.0f, 30.0f}))
					{
						instancesToRemove.push_back(i);
					}
				}
				i += 1;
			}
			
			for (int idx : instancesToRemove)
			{
				std::vector<size_t>::iterator it = scriptComp.mScriptInstances.begin();
				it += idx;
				scriptComp.mScriptInstances.erase(it);
			}
			instancesToRemove.clear();

			if(scriptComp.mScriptInstances.size() == 0)
			{
				InEntity->RemoveComponent<Razor::ScriptComponent>();
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
				if (Razor::RazorImGui::CheckBox("Use Gravity", &body.mbUseGravity)) {}

				if (Razor::RazorImGui::CheckBox("Is Static", &body.mbIsStatic)) {}

				if (Razor::RazorImGui::InputFloat("Mass", &body.mMass)) {}

				std::vector<const char*> motionTypes =
				{
					"Static",
					"Kinematic",
					"Dynamic"
				};
				int selectedIdx = 0;
				switch (body.mMotionType) 
				{
				case(Razor::EPhysicsMotionType::Kinematic):
					selectedIdx = 1;
					break;
				case(Razor::EPhysicsMotionType::Dynamic):
					selectedIdx = 2;
					break;
				}
				
				const char** data = motionTypes.data();

				if (Razor::RazorImGui::Combo("Motion Type", data, motionTypes.size(), &selectedIdx)) 
				{
					if (motionTypes[selectedIdx] == "Static")
					{
						body.mMotionType = Razor::EPhysicsMotionType::Static;
					}
					else if (motionTypes[selectedIdx] == "Kinematic")
					{
						body.mMotionType = Razor::EPhysicsMotionType::Kinematic;
					} 
					else if (motionTypes[selectedIdx] == "Dynamic")
					{
						body.mMotionType = Razor::EPhysicsMotionType::Dynamic;
					}
					
				}
				if(Razor::RazorImGui::Button("Remove Component", {200.0f, 30.0f}))
				{
					inEntity->RemoveComponent<Razor::BoxBody>();
				}
			}
		}
	}

	void ComponentImGui::DrawCamera(Razor::Ref<Razor::Entity> InEntity)
	{
		if (InEntity->HasComponent<Razor::Camera>())
		{
			Razor::Camera& camera = InEntity->GetComponent<Razor::Camera>();
			if (Razor::RazorImGui::CollapsingHeader("Camera"))
			{
				if(Razor::RazorImGui::Button("Remove Component", {200.0f, 30.0f}))
				{
					InEntity->RemoveComponent<Razor::Camera>();
				}
			}
		}
	}

	void ComponentImGui::DrawCollisionComponent(Razor::Ref<Razor::Entity> InEntity)
	{
		if (InEntity->HasComponent<Razor::CollisionComponent>())
		{
			if (Razor::RazorImGui::CollapsingHeader("Collision"))
			{
				Razor::CollisionComponent& collision = InEntity->GetComponent<Razor::CollisionComponent>();
				if(Razor::RazorImGui::CheckBox("Is Trigger", &collision.bIsTrigger))
				{
					
				}
				if(Razor::RazorImGui::Button("Remove Component", {200.0f, 30.0f}))
				{
					InEntity->RemoveComponent<Razor::CollisionComponent>();
				}
			}
		}
	}


	void ComponentImGui::DrawTextComponent(Razor::Ref<Razor::Entity> InEntity)
	{
		if (InEntity->HasComponent<Razor::Text>())
		{
			Razor::Text& text = InEntity->GetComponent<Razor::Text>();
			if (Razor::RazorImGui::CollapsingHeader("Text"))
			{
				char buffer[256];
				strncpy(buffer, text.GetText().c_str(), sizeof(buffer));
				if (Razor::RazorImGui::InputText("Content", buffer, sizeof(buffer)))
				{
					text.SetText(buffer);
				}
			
				Razor::RazorImGui::Text(text.mFontKey.GetKey().c_str());

				if (Razor::RazorImGui::Button("Pick"))
				{
					if (_mAssetPickerPopup == nullptr)
					{
						_mAssetPickerPopup = Razor::CreateRef<AssetPickerPopupWindow>(Razor::Engine::Get().GetAssetDirectory()->GetRootFolder());
					}
				}
				if (_mAssetPickerPopup)
				{
					if (_mAssetPickerPopup->Draw() == false)
					{
						if (_mAssetPickerPopup->GetSelectedAssetPath() != text.mFontKey.GetKey())
						{
							text.mFontKey = Razor::AssetKey{ _mAssetPickerPopup->GetSelectedAssetPath()};
						}
						_mAssetPickerPopup = nullptr;
					}
				}

				Razor::Ref<float[]> color = Razor::CreateRef<float[]>(3);
				color[0] = text.mColor.X;
				color[1] = text.mColor.Y;
				color[2] = text.mColor.Z;
				if (Razor::RazorImGui::ColorPicker("Text Color", color))
				{
					text.mColor = Razor::Vector3{color[0], color[1], color[2]};
				}
				if(Razor::RazorImGui::Button("Remove Component", {200.0f, 30.0f}))
				{
					InEntity->RemoveComponent<Razor::Text>();
				}
			}
		}
	}
}