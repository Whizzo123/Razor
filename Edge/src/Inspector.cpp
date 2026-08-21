#include "Inspector.h"
#include "Gui/ComponentImGui.h"
#include "EditorStorage.h"

namespace EdgeEditor
{
	// TODO don't really think we want this
	Inspector::Inspector() : Storage(std::make_shared<EditorStorage>())
	{

	}

	Inspector::Inspector(Razor::Ref<EditorStorage> Storage) : Storage(Storage)
	{
		ComponentImGui::SetAssetPickerPopup(_mAssetPickerPopup);
	}

	void Inspector::Render()
	{
		Razor::Engine& Engine = Razor::Engine::Get();

		bool bIsOpen;
		Razor::RazorImGui::Begin("Inspector", &bIsOpen, Razor::RazorGuiWindowFlags_MenuBar);
		Razor::RazorImGui::SetWindowSize(Razor::Vector2(200.0f, 200.0f));
		Razor::RazorImGui::Text("Hello Inspector, %d", 123);

		if (!Storage->SelectedEntity)
		{
			Razor::RazorImGui::End();
			return;
		}

		Razor::Ref<Razor::Scene> CurrentScene = Engine.mCurrentScene;
		
		if (Razor::RazorImGui::TreeNode(std::to_string((uint32_t)Storage->SelectedEntity->EntityHandle).c_str(), {0.0f, 0.0f}, true))
		{
			ComponentImGui::DrawComponents(Storage->SelectedEntity);
			Razor::RazorImGui::TreePop();
		}
		const char* PopupId = "Add Component Popup";
		if (Razor::RazorImGui::Button("Add Component"))
		{
			Razor::RazorImGui::OpenPopup(PopupId);	
		}
		if (Razor::RazorImGui::BeginPopup(PopupId))
		{
			if (Razor::RazorImGui::Button("Mesh Component"))
			{
				Storage->SelectedEntity->AddComponent<Razor::Mesh>();
			}
			if (Razor::RazorImGui::Button("Directional Light Component"))
			{
				Storage->SelectedEntity->AddComponent<Razor::DirectionalLight>();
			}
			if (Razor::RazorImGui::Button("Box Body Component"))
			{
				Storage->SelectedEntity->AddComponent<Razor::BoxBody>();
			}
			if (Razor::RazorImGui::Button("Camera Component"))
			{
				Storage->SelectedEntity->AddComponent<Razor::Camera>();
			}
			if (Razor::RazorImGui::Button("Collision Component"))
			{
				Storage->SelectedEntity->AddComponent<Razor::CollisionComponent>();
			}
			if (Razor::RazorImGui::Button("Text Component"))
			{
				std::string text = "Text";
				Storage->SelectedEntity->AddComponent<Razor::Text>(*Razor::Engine::Get().GetShaderForType(typeid(Razor::DefaultTextShader).name()), text);
			}
			for (const auto& component : Engine.GetScriptInterface().GetComponentTypes())
			{
				if (Razor::RazorImGui::Button(component.GetName().c_str()))
				{
					Storage->SelectedEntity->AddScriptComponent(component.GetName());
				}
			}
			Razor::RazorImGui::EndPopup();
		}
		Razor::RazorImGui::End();
	}

	void Inspector::CreateEntity()
	{
		Razor::Engine& Engine = Razor::Engine::Get();
		Razor::Ref<Razor::Entity> NewEntity = Engine.mCurrentScene->CreateEntity();
		NewEntity->AddComponent<Razor::Transform>();
	}

	void Inspector::CreateWidgetForProperty(const std::string& PropertyName, const std::string& PropertyValue)
	{

	}
}