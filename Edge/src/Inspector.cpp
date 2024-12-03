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
	if (ImGui::TreeNode("Entities"))
	{
		for (int EntityIndex = 0; EntityIndex < Engine.GetCoordinator()->GetCurrentEntityCount(); EntityIndex++)
		{
			ImGui::Text(std::to_string(EntityIndex).c_str());
		}
		ImGui::TreePop();
	}
	ImGui::End();
}

void Inspector::CreateEntity()
{
	Razor::Engine& Engine = Razor::Engine::Get();
	Razor::Entity NewEntity = Engine.CreateEntity();
	Engine.AddComponentToEntity(NewEntity, Razor::Transform());
}