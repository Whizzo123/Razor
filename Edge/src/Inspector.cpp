#include "Inspector.h"
#include "Gui/ComponentImGui.h"

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
		ImGui::Begin("Inspector", &bIsOpen, ImGuiWindowFlags_MenuBar);
		ImGui::SetWindowSize(ImVec2(200.0f, 200.0f));
		ImGui::Text("Hello Inspector, %d", 123);

		if (!Storage->SelectedEntity)
		{
			ImGui::End();
			return;
		}

		Razor::Ref<Razor::Scene> CurrentScene = Engine.CurrentScene;
		
		if (ImGui::TreeNode(std::to_string((uint32_t)Storage->SelectedEntity->EntityHandle).c_str()))
		{
			ComponentImGui::DrawComponents(Storage->SelectedEntity);
			ImGui::TreePop();
		}
		const char* PopupId = "Add Component Popup";
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup(PopupId);	
		}
		if (ImGui::BeginPopup(PopupId))
		{
			if (ImGui::Button("Mesh Component"))
			{
				Storage->SelectedEntity->AddComponent<Razor::Mesh>(Razor::CreateRef<Razor::Model>(Storage->DefaultModel));
			}
			if (ImGui::Button("Directional Light Component"))
			{
				Storage->SelectedEntity->AddComponent<Razor::DirectionalLight>();
			}
			ImGui::EndPopup();
		}
		ImGui::End();
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