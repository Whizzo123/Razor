#include "SceneView.h"
#include "EditorStorage.h"

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
		Razor::RazorImGui::Begin("Scene View", &bIsOpen, Razor::RazorGuiWindowFlags_MenuBar);
		Razor::RazorImGui::SetWindowSize(Razor::Vector2(200.0f, 200.0f));
		Razor::RazorImGui::Text("Hello Scene View, %d", 123);

		Razor::Ref<Razor::Scene> CurrentScene = Engine.CurrentScene;

		if(Razor::RazorImGui::Button("Add Entity"))
		{
			CurrentScene->CreateEntity();
		}

		if (Razor::RazorImGui::TreeNode("Entities", {0.0f, 0.0f}, true))
		{
			// Need wrapper for the view type not lovely to have
			auto View = CurrentScene->GetEntitiesWithComponents<Razor::Transform>();
			for (auto Entity : View)
			{
				if (Razor::RazorImGui::Button(std::to_string((uint32_t)Entity), {200.0f, 30.0f}))
				{
					Storage->SelectedEntity = CurrentScene->GetEntity(Entity);
				}
			}
			Razor::RazorImGui::TreePop();
		}
		Razor::RazorImGui::End();
	}
}
