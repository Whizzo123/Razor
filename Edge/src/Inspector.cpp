#include "Inspector.h"
#include "Gui/ComponentImGui.h"

namespace EdgeEditor
{
	void Inspector::Render()
	{
		Razor::Engine& Engine = Razor::Engine::Get();

		bool bIsOpen;
		ImGui::Begin("Inspector", &bIsOpen, ImGuiWindowFlags_MenuBar);
		ImGui::SetWindowSize(ImVec2(200.0f, 200.0f));
		ImGui::Text("Hello Inspector, %d", 123);
		if (ImGui::Button("Create Entity", ImVec2(50.f, 25.f)))
		{
			CreateEntity();
		}

		Razor::Ref<Razor::Scene> CurrentScene = Engine.CurrentScene;

		if (ImGui::TreeNode("Entities"))
		{
			// Need wrapper for the view type not lovely to have
			auto View = CurrentScene->GetEntitiesWithComponents<Razor::Transform>();
			for (auto Entity : View)
			{
				if (ImGui::CollapsingHeader(std::to_string((uint32_t)Entity).c_str()))
				{
					ComponentImGui::DrawComponents(CurrentScene->GetEntity(Entity));
				}
			}
			ImGui::TreePop();
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