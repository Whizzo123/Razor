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

		Razor::Ref<Razor::Scene> CurrentScene = Engine.CurrentScene;
		
		if (Razor::RazorImGui::TreeNode(std::to_string((uint32_t)Storage->SelectedEntity->EntityHandle).c_str()))
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
				Storage->SelectedEntity->AddComponent<Razor::Mesh>(Razor::CreateRef<Razor::Model>(Storage->DefaultModel));
			}
			if (Razor::RazorImGui::Button("Directional Light Component"))
			{
				Storage->SelectedEntity->AddComponent<Razor::DirectionalLight>();
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
		Razor::Ref<Razor::Entity> NewEntity = Engine.CurrentScene->CreateEntity();
		NewEntity->AddComponent<Razor::Transform>();
	}

	void Inspector::CreateWidgetForProperty(const std::string& PropertyName, const std::string& PropertyValue)
	{

	}
}