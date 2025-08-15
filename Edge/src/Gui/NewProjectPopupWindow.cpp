#include "NewProjectPopupWindow.h"
#include "../FileIO/ProjectSerializer.h"
//TODO move to Razor.h
#include "misc/cpp/imgui_stdlib.h"

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
		if (ImGui::BeginPopupModal("New Project Window", nullptr))
		{
			//ImGui::InputText("Project Name", &NewProject->ProjectName);
			if (ImGui::Button("Create"))
			{
				EdgeEditor::ProjectSerializer::Serialize(MagicalPathToFixWithActualSelectedPathSoon, NewProject);
				/*TODO actually load project up*/
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{ 
				Close();
				bIsOpen = false;
			}
			ImGui::EndPopup();
		}
		return bIsOpen;
	}
	void NewProjectPopupWindow::Open()
	{
		NewProject = Razor::CreateRef<Project>();
		ImGui::OpenPopup("New Project Window");
	}
	void NewProjectPopupWindow::Close()
	{
		ImGui::CloseCurrentPopup();
	}
}