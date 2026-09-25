#include "SystemView.h"
#include "EditorStorage.h"
#include "Gui/Layout/EditorWindowRegistry.h"
#include <Razor.h>

namespace EdgeEditor
{
	SystemView::SystemView()
	{
		RegisterEditorWindow<SystemView>();
	}

	SystemView::SystemView(Razor::Ref<EditorStorage> storage) : _mStorage(storage)
	{
		RegisterEditorWindow<SystemView>();
	}

	void SystemView::Render()
	{
		Razor::Engine& Engine = Razor::Engine::Get();

		bool bIsOpen;
		Razor::RazorImGui::Begin("System View", &bIsOpen, Razor::RazorGuiWindowFlags_MenuBar);
		Razor::RazorImGui::SetWindowSize(Razor::Vector2(200.0f, 200.0f));

		Razor::Ref<Razor::Scene> CurrentScene = Engine.mCurrentScene;
		
		std::vector<Razor::ScriptClass> SystemClasses = Engine.GetScriptInterface().GetSystemTypes();
		std::vector<const char*> SystemClassNames(SystemClasses.size());
		if (SystemClassNames.size() == 0)
		{
			Razor::RazorImGui::End();
			return;
		}
		for(int i = 0; i < SystemClassNames.size(); i++)
		{
			SystemClassNames[i] = SystemClasses[i].GetName().c_str();
		}
		int selectedIdx = 0;
		const char** data = SystemClassNames.data();
		if (Razor::RazorImGui::Combo("Add System", data, SystemClassNames.size(),  &selectedIdx)) {}

		if (Razor::RazorImGui::Button("Add"))
		{
			CurrentScene->CreateSystemInstance(Engine.GetScriptInterface().GetType(SystemClassNames[selectedIdx]));
		}

		if (Razor::RazorImGui::TreeNode("Systems", {0.0f, 0.0f}, true))
		{
			for (const auto& name : CurrentScene->GetSystemNames())
			{
				Razor::RazorImGui::Text(name.c_str());
			}
			Razor::RazorImGui::TreePop();
		}
		Razor::RazorImGui::End();
	}
}