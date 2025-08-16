#include "NewProjectPopupWindow.h"
#include "../FileIO/ProjectSerializer.h"
#include "../Project.h"

namespace EdgeEditor
{
	bool NewProjectPopupWindow::Draw()
	{
		const std::string MagicalPathToFixWithActualSelectedPathSoon = "../../Sandboxes/";

		bool bIsOpen = true;
		if (NewProject == nullptr)
		{
			Open();
		}
		if (Razor::RazorImGui::BeginPopupModal("New Project Window", nullptr))
		{
			//ImGui::InputText("Project Name", &NewProject->ProjectName);
			if (Razor::RazorImGui::Button("Create"))
			{
				EdgeEditor::ProjectSerializer::Serialize(MagicalPathToFixWithActualSelectedPathSoon, NewProject);
				/*TODO actually load project up*/
				Razor::RazorImGui::CloseCurrentPopup();
			}
			Razor::RazorImGui::SameLine();
			if (Razor::RazorImGui::Button("Cancel"))
			{ 
				Close();
				bIsOpen = false;
			}
			Razor::RazorImGui::EndPopup();
		}
		return bIsOpen;
	}
	void NewProjectPopupWindow::Open()
	{
		NewProject = Razor::CreateRef<Project>();
		Razor::RazorImGui::OpenPopup("New Project Window");
	}
	void NewProjectPopupWindow::Close()
	{
		Razor::RazorImGui::CloseCurrentPopup();
	}
}