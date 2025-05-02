#include "SceneView.h"


namespace EdgeEditor
{
	SceneView::SceneView() : Storage(std::make_shared<EditorStorage>())
	{

	}

	SceneView::SceneView(Razor::Ref<EditorStorage> Storage) : Storage(Storage)
	{

	}

	void SceneView::Render()
	{
		Razor::Engine& Engine = Razor::Engine::Get();

		bool bIsOpen;
		ImGui::Begin("Scene View", &bIsOpen, ImGuiWindowFlags_MenuBar);
		ImGui::SetWindowSize(ImVec2(200.0f, 200.0f));
		ImGui::Text("Hello Scene View, %d", 123);

		Razor::Ref<Razor::Scene> CurrentScene = Engine.CurrentScene;

		if (ImGui::TreeNode("Entities"))
		{
			// Need wrapper for the view type not lovely to have
			auto View = CurrentScene->GetEntitiesWithComponents<Razor::Transform>();
			for (auto Entity : View)
			{
				if (ImGui::Button(std::to_string((uint32_t)Entity).c_str()))
				{
					Storage->SelectedEntity = CurrentScene->GetEntity(Entity);
				}
			}
			ImGui::TreePop();
		}
		ImGui::End();
	}
}
