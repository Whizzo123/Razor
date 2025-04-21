#include "Inspector.h"


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
			if (ImGui::Button(std::to_string((uint32_t)Entity).c_str(), ImVec2(50.f, 25.f)))
			{
				/*std::vector<const char*> ComponentTypeNames = Coordinator->GetComponentsForEntity(EntityIndex);
				for (const char*& Name : ComponentTypeNames)
				{
					if (Name == typeid(Razor::Transform).name())
					{*/
						// TODO do we want to consider some sort of reflection based approach to the typing i.e a map of typename() to T
						/*Razor::Transform TransformComp = Coordinator->GetComponent<Razor::Transform>(EntityIndex);
						std::unordered_map<std::string, std::string> PropertiesMap = TransformComp.Serialize();
						for (auto const& Property : PropertiesMap)
						{
							CreateWidgetForProperty(Property.first, Property.second);
						}*/
					//}
				//}
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